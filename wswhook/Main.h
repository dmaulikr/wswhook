#pragma once

#include <Windows.h>
#include <stdio.h>
#include <detours.h>
#include <stddef.h>
#include <assert.h>
#include <stddef.h>
#include <vector>
#include <math.h>
#include <process.h>

#include <sys/timeb.h>

#include "Sdk.h"
#include "Tools.h"
#include "Engine.h"
#include "Math.h"

using namespace std;

extern cgame_import_t* imports;
extern cgame_export_t* exports;