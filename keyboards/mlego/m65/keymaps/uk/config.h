/*
Copyright 2021-2022 Alin M Elena <alinm.elena@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once


#define UNICODE_SELECTED_MODES UNICODE_MODE_LINUX
//#define UNICODE_SELECTED_MODES UNICODE_MODE_LINUX, UNICODE_MODE_MACOS, UNICODE_MODE_WINCOMPOSE

#ifdef WPM_ENABLE
#define WPM_SAMPLE_SECONDS 10
#define WPM_SAMPLE_PERIODS 100
#define WPM_ALLOW_COUNT_REGRESSION
#endif
