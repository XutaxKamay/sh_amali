// pch.h : Il s'agit d'un fichier d'en-tête précompilé.
// Les fichiers listés ci-dessous sont compilés une seule fois, ce qui améliore
// les performances de génération des futures builds. Cela affecte également les
// performances d'IntelliSense, notamment la complétion du code et de nombreuses
// fonctionnalités de navigation du code. Toutefois, les fichiers listés ici
// sont TOUS recompilés si l'un d'entre eux est mis à jour entre les builds.
// N'ajoutez pas de fichiers fréquemment mis à jour ici, car cela annule les
// gains de performance.

#ifndef PCH_H
#define PCH_H

// ajouter les en-têtes à précompiler ici
#include "framework.h"
#pragma warning(push)
#pragma warning(disable : 26495)
#include <d3d9.h>
#include <d3dx9.h>
#pragma warning(pop)
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "imgui_internal.h"
#include "menu.h"
#include <iostream>
#include <string>
#include <thread>
#include <utils.h>

void mainThread(HMODULE hModule);

#endif // PCH_H
