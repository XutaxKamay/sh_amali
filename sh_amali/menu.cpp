#include "pch.h"

static bool show_app_style_editor = false;
static bool show_menu = false;

struct process_info_t
{
    uintptr_t base;
    size_t size;
};

process_info_t getProcessInfos()
{
    process_info_t p_info;

    p_info.base = reinterpret_cast<uintptr_t>(GetModuleHandleA("game.exe"));

    auto gameDosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(p_info.base);

    auto gameNtHeader = reinterpret_cast<PIMAGE_NT_HEADERS>(
        p_info.base + gameDosHeader->e_lfanew);

    p_info.size = gameNtHeader->OptionalHeader.SizeOfImage;

    return p_info;
}

uintptr_t searchInProcess(const std::string& strPattern)
{
    static auto g_process = getProcessInfos();

    auto ptrs = memory_info::search(strPattern,
                                    g_process.base,
                                    g_process.base + g_process.size);

    if (ptrs.size() != 0)
        return ptrs[0];

    return 0;
}

// ------------------------ ADDRESSES ----------------------------
static uintptr_t skillCutAddr = searchInProcess(
    "8B 81 ?? ?? ?? ?? 83 F8 01 74 12");

static uintptr_t xrayAddr = searchInProcess("74 05 80 3E 0A") + 7;

static uintptr_t fuckcooldownAddr = searchInProcess(
    "74 36 68 ?? ?? ?? ?? 6A 11");

static uintptr_t fuckzoomlimitAddr = searchInProcess("85 C0 8B D1");

static uintptr_t fuckstunsAddr = searchInProcess("74 10 8A 54 24 0C");

static uintptr_t wallclimbAddr = searchInProcess("0F 85 ?? ?? ?? ?? D9 41 18");

static uintptr_t fuckrangeAddr = searchInProcess("89 44 24 14 DB 44 24 14 D8 "
                                                 "44 24 4C D9 5C 24 4C A1") -
                                 9;

static uintptr_t fuckrange2Addr = *reinterpret_cast<uintptr_t*>(fuckrangeAddr +
                                                                5);

static uintptr_t mobsHpAddr = searchInProcess("85 C0 74 3E 8B 6C 24 18");

static uintptr_t fuckJumpingAddr = searchInProcess("3B 3D ?? ?? ?? ?? 0F 84 ?? "
                                                   "?? ?? ?? 8A 86") +
                                   6;

static uintptr_t fuckWalkingAddr = searchInProcess(
    "89 88 ?? ?? ?? ?? C2 04 00");

static uintptr_t fuckSafezonesAddr = searchInProcess(
    "64 61 74 61 2F 77 6F 72 6C 64 2F 54 61 63 74 69 63 73 5A 6F 6E 65 2E 7A "
    "6F 6E 00");

static uintptr_t fuckRcdAddr = searchInProcess("74 3C 68 ?? ?? ?? ?? 6A 11");
// ---------------------------------------------------------------

bool safeWriteBytes(uintptr_t address, std::vector<byte> bytes)
{
    DWORD dwOldProt;

    if (VirtualProtect(reinterpret_cast<ptr_t>(address),
                       bytes.size(),
                       PAGE_EXECUTE_READWRITE,
                       &dwOldProt))
    {
        memcpy(reinterpret_cast<ptr_t>(address), bytes.data(), bytes.size());

        return VirtualProtect(reinterpret_cast<ptr_t>(address),
                              bytes.size(),
                              PAGE_EXECUTE_READWRITE,
                              &dwOldProt);
    }

    return false;
}

void skillCut()
{
    static bool bActivated = false;

    if (ImGui::Checkbox("Skill Cut", &bActivated))
    {
        if (skillCutAddr == 0)
        {
            console::Msg<FOREGROUND_RED>("Couldn't find Skill Cut\n");
            return;
        }

        static std::vector<byte> oldBytes(5);

        if (bActivated)
        {
            memcpy(oldBytes.data(),
                   reinterpret_cast<ptr_t>(skillCutAddr),
                   oldBytes.size());

            safeWriteBytes(skillCutAddr, {0x90, 0x90, 0x90, 0x90, 0x90});

            console::Msg<FOREGROUND_GREEN>("Enabled Skill Cut\n");
        }
        else
        {
            safeWriteBytes(skillCutAddr, oldBytes);
            console::Msg<FOREGROUND_BLUE>("Disabled Skill Cut\n");
        }
    }
}

void xray()
{
    static bool bActivated = false;

    if (ImGui::Checkbox("X-Ray", &bActivated))
    {
        if (xrayAddr == 0)
        {
            console::Msg<FOREGROUND_RED>("Couldn't find X-Ray\n");
            return;
        }

        static std::vector<byte> oldBytes(10);

        if (bActivated)
        {
            memcpy(oldBytes.data(),
                   reinterpret_cast<ptr_t>(xrayAddr),
                   oldBytes.size());

            safeWriteBytes(
                xrayAddr,
                {0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90});

            console::Msg<FOREGROUND_GREEN>("Enabled X-Ray\n");
        }
        else
        {
            safeWriteBytes(xrayAddr, oldBytes);
            console::Msg<FOREGROUND_BLUE>("Disabled X-Ray\n");
        }
    }
}

void fuckCoolDown()
{
    static bool bActivated = false;

    if (ImGui::Checkbox("Fuck Cooldown", &bActivated))
    {
        if (fuckcooldownAddr == 0)
        {
            console::Msg<FOREGROUND_RED>("Couldn't find Cooldown\n");
            return;
        }

        static std::vector<byte> oldBytes(1);

        if (bActivated)
        {
            memcpy(oldBytes.data(),
                   reinterpret_cast<ptr_t>(fuckcooldownAddr),
                   oldBytes.size());

            safeWriteBytes(fuckcooldownAddr, {0xEB});

            console::Msg<FOREGROUND_GREEN>("Enabled Cooldown\n");
        }
        else
        {
            safeWriteBytes(fuckcooldownAddr, oldBytes);
            console::Msg<FOREGROUND_BLUE>("Disabled Cooldown\n");
        }
    }
}

void fuckZoomLimit()
{
    static bool bActivated = false;

    if (ImGui::Checkbox("Fuck Zoom Limit", &bActivated))
    {
        if (fuckzoomlimitAddr == 0)
        {
            console::Msg<FOREGROUND_RED>("Couldn't find Zoom Limit\n");
            return;
        }

        static std::vector<byte> oldBytes(0x64);

        if (bActivated)
        {
            memcpy(oldBytes.data(),
                   reinterpret_cast<ptr_t>(fuckzoomlimitAddr),
                   oldBytes.size());

            safeWriteBytes(fuckzoomlimitAddr, {0x90});
            safeWriteBytes(fuckzoomlimitAddr + 1, {0x90});

            safeWriteBytes(fuckzoomlimitAddr + 0x10, {0x90});
            safeWriteBytes(fuckzoomlimitAddr + 0x10 + 1, {0x90});

            safeWriteBytes(fuckzoomlimitAddr + 0x18, {0x90});
            safeWriteBytes(fuckzoomlimitAddr + 0x18 + 1, {0x90});

            safeWriteBytes(fuckzoomlimitAddr + 0x18 + 10, {0x90});
            safeWriteBytes(fuckzoomlimitAddr + 0x18 + 10 + 1, {0x90});

            console::Msg<FOREGROUND_GREEN>("Enabled Zoom Limit\n");
        }
        else
        {
            safeWriteBytes(fuckzoomlimitAddr, oldBytes);
            console::Msg<FOREGROUND_BLUE>("Disabled Zoom Limit\n");
        }
    }
}

void fuckStuns()
{
    static bool bActivated = false;

    if (ImGui::Checkbox("Fuck Stuns", &bActivated))
    {
        if (fuckstunsAddr == 0)
        {
            console::Msg<FOREGROUND_RED>("Couldn't find Stuns\n");
            return;
        }

        static std::vector<byte> oldBytes(1);

        if (bActivated)
        {
            memcpy(oldBytes.data(),
                   reinterpret_cast<ptr_t>(fuckstunsAddr),
                   oldBytes.size());

            safeWriteBytes(fuckstunsAddr, {0xEB});

            console::Msg<FOREGROUND_GREEN>("Enabled Stuns\n");
        }
        else
        {
            safeWriteBytes(fuckstunsAddr, oldBytes);
            console::Msg<FOREGROUND_BLUE>("Disabled Stuns\n");
        }
    }
}

void wallClimb()
{
    static bool bActivated = false;

    if (ImGui::Checkbox("Wall Climb", &bActivated))
    {
        if (wallclimbAddr == 0)
        {
            console::Msg<FOREGROUND_RED>("Couldn't find Wall Climb\n");
            return;
        }

        static std::vector<byte> oldBytes(2);

        if (bActivated)
        {
            memcpy(oldBytes.data(),
                   reinterpret_cast<ptr_t>(wallclimbAddr),
                   oldBytes.size());

            safeWriteBytes(wallclimbAddr, {0x90, 0xE9});

            console::Msg<FOREGROUND_GREEN>("Enabled Wall Climb\n");
        }
        else
        {
            safeWriteBytes(wallclimbAddr, oldBytes);
            console::Msg<FOREGROUND_BLUE>("Disabled Wall Climb\n");
        }
    }
}

void fuckRange()
{
    static bool bActivated = false;

    if (ImGui::Checkbox("Fuck Range", &bActivated))
    {
        if (fuckrange2Addr == 0 || fuckrangeAddr == 0)
        {
            console::Msg<FOREGROUND_RED>("Couldn't find Range\n");
            return;
        }

        static std::vector<byte> oldBytes(4);
        static std::vector<byte> oldBytes2(4);
        static std::vector<byte> oldBytes3(2);

        if (bActivated)
        {
            memcpy(oldBytes.data(),
                   reinterpret_cast<ptr_t>(fuckrangeAddr),
                   oldBytes.size());

            memcpy(oldBytes2.data(),
                   reinterpret_cast<ptr_t>(fuckrangeAddr + 0xADD),
                   oldBytes2.size());

            safeWriteBytes(fuckrangeAddr, {0x90, 0x90, 0x90, 0x90});
            safeWriteBytes(fuckrangeAddr + 0xADD, {0x90, 0x90, 0x90, 0x90});

            memcpy(oldBytes3.data(),
                   reinterpret_cast<ptr_t>(fuckrangeAddr),
                   oldBytes3.size());

            safeWriteBytes(fuckrange2Addr, {0xFF, 0xFF});

            console::Msg<FOREGROUND_GREEN>("Enabled Range\n");
        }
        else
        {
            safeWriteBytes(fuckrangeAddr, oldBytes);
            safeWriteBytes(fuckrangeAddr + 0xADD, oldBytes2);
            safeWriteBytes(fuckrange2Addr, oldBytes3);
            console::Msg<FOREGROUND_BLUE>("Disabled Range\n");
        }
    }
}

void mobsHp()
{
    static bool bActivated = false;

    if (ImGui::Checkbox("Mobs HP", &bActivated))
    {
        if (mobsHpAddr == 0)
        {
            console::Msg<FOREGROUND_RED>("Couldn't find Mobs HP\n");
            return;
        }

        static std::vector<byte> oldBytes(4);

        if (bActivated)
        {
            memcpy(oldBytes.data(),
                   reinterpret_cast<ptr_t>(mobsHpAddr),
                   oldBytes.size());

            safeWriteBytes(mobsHpAddr, {0x90, 0x90, 0x90, 0x90});

            console::Msg<FOREGROUND_GREEN>("Enabled Mobs HP\n");
        }
        else
        {
            safeWriteBytes(mobsHpAddr, oldBytes);
            console::Msg<FOREGROUND_BLUE>("Disabled Mobs HP\n");
        }
    }
}

void fuckJumping()
{
    static bool bActivated = false;

    if (ImGui::Checkbox("Fuck Jumping", &bActivated))
    {
        if (fuckJumpingAddr == 0)
        {
            console::Msg<FOREGROUND_RED>("Couldn't find Jumping\n");
            return;
        }

        static std::vector<byte> oldBytes(6);

        if (bActivated)
        {
            memcpy(oldBytes.data(),
                   reinterpret_cast<ptr_t>(fuckJumpingAddr),
                   oldBytes.size());

            safeWriteBytes(fuckJumpingAddr,
                           {0xE9, 0x6E, 0x02, 0x00, 0x00, 0x90});

            console::Msg<FOREGROUND_GREEN>("Enabled Jumping\n");
        }
        else
        {
            safeWriteBytes(fuckJumpingAddr, oldBytes);
            console::Msg<FOREGROUND_BLUE>("Disabled Jumping\n");
        }
    }
}

void fuckWalking()
{
    static bool bActivated = false;

    if (ImGui::Checkbox("Fuck Walking", &bActivated))
    {
        if (fuckWalkingAddr == 0)
        {
            console::Msg<FOREGROUND_RED>("Couldn't find Walking\n");
            return;
        }

        static std::vector<byte> oldBytes(6);

        if (bActivated)
        {
            memcpy(oldBytes.data(),
                   reinterpret_cast<ptr_t>(fuckWalkingAddr),
                   oldBytes.size());

            safeWriteBytes(fuckWalkingAddr,
                           {0x90, 0x90, 0x90, 0x90, 0x90, 0x90});

            console::Msg<FOREGROUND_GREEN>("Enabled Walking\n");
        }
        else
        {
            safeWriteBytes(fuckWalkingAddr, oldBytes);
            console::Msg<FOREGROUND_BLUE>("Disabled Walking\n");
        }
    }
}

void fuckSafeZones()
{
    static bool bActivated = false;

    if (ImGui::Checkbox("Fuck Safe Zones", &bActivated))
    {
        if (fuckSafezonesAddr == 0)
        {
            console::Msg<FOREGROUND_RED>("Couldn't find Safe Zones\n");
            return;
        }

        static std::vector<byte> oldBytes(6);

        if (bActivated)
        {
            memcpy(oldBytes.data(),
                   reinterpret_cast<ptr_t>(fuckSafezonesAddr + 11),
                   oldBytes.size());

            safeWriteBytes(fuckSafezonesAddr + 11,
                           {0x00,
                            0x00,
                            0x00,
                            0x00,
                            0x00,
                            0x00,
                            0x00,
                            0x00,
                            0x00,
                            0x00,
                            0x00,
                            0x00,
                            0x00,
                            0x00,
                            0x00});

            console::Msg<FOREGROUND_GREEN>("Enabled Fuck Safe Zones\n");
        }
        else
        {
            safeWriteBytes(fuckSafezonesAddr + 11, oldBytes);
            console::Msg<FOREGROUND_BLUE>("Disabled Fuck Safe Zones\n");
        }
    }
}

void fuckRcd()
{
    static bool bActivated = false;

    if (ImGui::Checkbox("Fuck RCD", &bActivated))
    {
        if (fuckRcdAddr == 0)
        {
            console::Msg<FOREGROUND_RED>("Couldn't find RCD\n");
            return;
        }

        static std::vector<byte> oldBytes(1);

        if (bActivated)
        {
            memcpy(oldBytes.data(),
                   reinterpret_cast<ptr_t>(fuckRcdAddr),
                   oldBytes.size());

            safeWriteBytes(fuckRcdAddr, {0xEB});
            console::Msg<FOREGROUND_GREEN>("Enabled RCD\n");
        }
        else
        {
            safeWriteBytes(fuckRcdAddr, oldBytes);
            console::Msg<FOREGROUND_BLUE>("Disabled RCD\n");
        }
    }
}

void CherryTheme()
{
    auto& style = ImGui::GetStyle();
    auto& colors = style.Colors;

    colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 0.29f, 0.48f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(1.00f, 0.59f, 0.98f, 0.40f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(1.00f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(1.00f, 0.29f, 0.48f, 0.8f);
    colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 0.59f, 0.98f, 0.8f);
    colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 0.52f, 0.88f, 0.8f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 0.59f, 0.98f, 0.8f);
    colors[ImGuiCol_Button] = ImVec4(1.00f, 0.59f, 0.98f, 0.40f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(1.00f, 0.59f, 0.98f, 0.8f);
    colors[ImGuiCol_ButtonActive] = ImVec4(1.00f, 0.53f, 0.98f, 0.8f);
    colors[ImGuiCol_Header] = ImVec4(1.00f, 0.59f, 0.98f, 0.31f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(1.00f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(1.00f, 0.59f, 0.98f, 0.8f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(1.00f, 0.40f, 0.75f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(1.00f, 0.40f, 0.75f, 0.8f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 0.59f, 0.98f, 0.25f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_Tab] = ImVec4(1.00f, 0.35f, 0.58f, 0.86f);
    colors[ImGuiCol_TabHovered] = ImVec4(1.00f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_TabActive] = ImVec4(1.00f, 0.41f, 0.68f, 0.8f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(1.00f, 0.26f, 0.42f, 0.8f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(1.00f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.59f, 0.98f, 0.8f);
}

void styleEditor()
{
    static bool initStyles = false;

    if (!initStyles)
    {
        CherryTheme();
        initStyles = true;
    }

    if (show_app_style_editor)
    {
        ImGui::Begin("Style Editor", &show_app_style_editor);
        ImGui::ShowStyleEditor();
        ImGui::End();
    }
}

void menuHack()
{
    if (show_menu)
    {
        ImGui::Begin("Hack menu", &show_menu);

        skillCut();
        xray();
        fuckCoolDown();
        fuckZoomLimit();
        fuckStuns();
        wallClimb();
        fuckRange();
        mobsHp();
        fuckJumping();
        fuckWalking();
        fuckSafeZones();
        fuckRcd();

        ImGui::End();
    }
}

void drawMenu()
{
    styleEditor();
    menuHack();

    if (ImGui::BeginMainMenuBar())
    {
        ImGui::Text("Amali princess hack (press enter to close/open)	"
                    "		");

        if (ImGui::BeginMenu("Menus"))
        {
            ImGui::MenuItem("Toggle hack menu", NULL, &show_menu);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Options"))
        {
            ImGui::MenuItem("Style Editor", NULL, &show_app_style_editor);
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}
