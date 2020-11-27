/*

Copyright (c) 2020 Boris I. Bendovsky (bibendovsky@hotmail.com) and Contributors.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/


#include "EFX-Util.h"

#include <math.h>
#include <stddef.h>


static const float EAXREVERB_MINENVIRONMENTSIZE = 1.0F;
static const float EAXREVERB_MAXENVIRONMENTSIZE = 100.0F;

static const float EAXREVERB_MINDECAYTIME = 0.1F;
static const float EAXREVERB_MAXDECAYTIME = 20.0F;

static const long EAXREVERB_MINREFLECTIONS = -10000L;
static const long EAXREVERB_MAXREFLECTIONS = 1000L;

static const float EAXREVERB_MINREFLECTIONSDELAY = 0.0F;
static const float EAXREVERB_MAXREFLECTIONSDELAY = 0.3F;

static const long EAXREVERB_MINREVERB = -10000L;
static const long EAXREVERB_MAXREVERB = 2000L;

static const float EAXREVERB_MINREVERBDELAY = 0.0F;
static const float EAXREVERB_MAXREVERBDELAY = 0.1F;

static const float EAXREVERB_MINECHOTIME = 0.075F;
static const float EAXREVERB_MAXECHOTIME = 0.25F;

static const float EAXREVERB_MINMODULATIONTIME = 0.04F;
static const float EAXREVERB_MAXMODULATIONTIME = 4.0F;

static const unsigned long EAXREVERBFLAGS_DECAYTIMESCALE = 0x00000001UL;
static const unsigned long EAXREVERBFLAGS_REFLECTIONSSCALE = 0x00000002UL;
static const unsigned long EAXREVERBFLAGS_REFLECTIONSDELAYSCALE = 0x00000004UL;
static const unsigned long EAXREVERBFLAGS_REVERBSCALE = 0x00000008UL;
static const unsigned long EAXREVERBFLAGS_REVERBDELAYSCALE = 0x00000010UL;
static const unsigned long EAXREVERBFLAGS_ECHOTIMESCALE = 0x00000040UL;
static const unsigned long EAXREVERBFLAGS_MODULATIONTIMESCALE = 0x00000080UL;
static const unsigned long EAXREVERBFLAGS_DECAYHFLIMIT = 0x00000020UL;


static long oal_efx_util_clampl(
	long value,
	long min_value,
	long max_value)
{
	if (value < min_value)
	{
		return min_value;
	}
	else if (value > max_value)
	{
		return max_value;
	}
	else
	{
		return value;
	}
}

static float oal_efx_util_clampf(
	float value,
	float min_value,
	float max_value)
{
	if (value < min_value)
	{
		return min_value;
	}
	else if (value > max_value)
	{
		return max_value;
	}
	else
	{
		return value;
	}
}


float MilliBelToLinearGain(
	float x)
{
	if (x <= -10000.0F)
	{
		return 0.0F;
	}
	else
	{
		return powf(10.0F, x / 2000.0F);
	}
}


void ConvertReverbParameters(
	EAXREVERBPROPERTIES* pEAXProp,
	EFXEAXREVERBPROPERTIES* pEFXEAXReverb)
{
	if (pEAXProp == NULL || pEFXEAXReverb == NULL)
	{
		return;
	}

	pEFXEAXReverb->flDensity = fminf(powf(pEAXProp->flEnvironmentSize, 3.0F) / 16.0F, 1.0F);
	pEFXEAXReverb->flDiffusion = pEAXProp->flEnvironmentDiffusion;
	pEFXEAXReverb->flGain = MilliBelToLinearGain((float)pEAXProp->lRoom);
	pEFXEAXReverb->flGainHF = MilliBelToLinearGain((float)pEAXProp->lRoomHF);
	pEFXEAXReverb->flGainLF = MilliBelToLinearGain((float)pEAXProp->lRoomLF);
	pEFXEAXReverb->flDecayTime = pEAXProp->flDecayTime;
	pEFXEAXReverb->flDecayHFRatio = pEAXProp->flDecayHFRatio;
	pEFXEAXReverb->flDecayLFRatio = pEAXProp->flDecayLFRatio;
	pEFXEAXReverb->flReflectionsGain = MilliBelToLinearGain((float)pEAXProp->lReflections);
	pEFXEAXReverb->flReflectionsDelay = pEAXProp->flReflectionsDelay;
	pEFXEAXReverb->flReflectionsPan[0] = pEAXProp->vReflectionsPan.x;
	pEFXEAXReverb->flReflectionsPan[1] = pEAXProp->vReflectionsPan.y;
	pEFXEAXReverb->flReflectionsPan[2] = pEAXProp->vReflectionsPan.z;
	pEFXEAXReverb->flLateReverbGain = MilliBelToLinearGain((float)pEAXProp->lReverb);
	pEFXEAXReverb->flLateReverbDelay = pEAXProp->flReverbDelay;
	pEFXEAXReverb->flLateReverbPan[0] = (pEAXProp->vReverbPan).x;
	pEFXEAXReverb->flLateReverbPan[1] = (pEAXProp->vReverbPan).y;
	pEFXEAXReverb->flLateReverbPan[2] = (pEAXProp->vReverbPan).z;
	pEFXEAXReverb->flEchoTime = pEAXProp->flEchoTime;
	pEFXEAXReverb->flEchoDepth = pEAXProp->flEchoDepth;
	pEFXEAXReverb->flModulationTime = pEAXProp->flModulationTime;
	pEFXEAXReverb->flModulationDepth = pEAXProp->flModulationDepth;
	pEFXEAXReverb->flAirAbsorptionGainHF = MilliBelToLinearGain(pEAXProp->flAirAbsorptionHF);
	pEFXEAXReverb->flHFReference = pEAXProp->flHFReference;
	pEFXEAXReverb->flLFReference = pEAXProp->flLFReference;
	pEFXEAXReverb->flRoomRolloffFactor = pEAXProp->flRoomRolloffFactor;
	pEFXEAXReverb->iDecayHFLimit = ((pEAXProp->ulFlags & EAXREVERBFLAGS_DECAYHFLIMIT) != 0);
}

void ConvertObstructionParameters(
	EAXOBSTRUCTIONPROPERTIES* pObProp,
	EFXLOWPASSFILTER* pDirectLowPassFilter)
{
	if (pObProp == NULL || pDirectLowPassFilter == NULL)
	{
		return;
	}

	const float obstruction_f = (float)pObProp->lObstruction;

	pDirectLowPassFilter->flGain = MilliBelToLinearGain(obstruction_f * pObProp->flObstructionLFRatio);
	pDirectLowPassFilter->flGainHF = MilliBelToLinearGain(obstruction_f);
}

void ConvertExclusionParameters(
	EAXEXCLUSIONPROPERTIES* pExProp,
	EFXLOWPASSFILTER* pSendLowPassFilter)
{
	if (pExProp == NULL || pSendLowPassFilter == NULL)
	{
		return;
	}

	const float exclusion_f = (float)pExProp->lExclusion;

	pSendLowPassFilter->flGain = MilliBelToLinearGain(exclusion_f * pExProp->flExclusionLFRatio);
	pSendLowPassFilter->flGainHF = MilliBelToLinearGain(exclusion_f);
}


void ConvertOcclusionParameters(
	EAXOCCLUSIONPROPERTIES* pOcProp,
	EFXLOWPASSFILTER* pDirectLowPassFilter,
	EFXLOWPASSFILTER* pSendLowPassFilter)
{
	if (pOcProp == NULL || pDirectLowPassFilter == NULL || pSendLowPassFilter == NULL)
	{
		return;
	}

	const float occlusion_f = (float)pOcProp->lOcclusion;

	// Direct.
	//
	{
		const float ratio_1 = (pOcProp->flOcclusionLFRatio + pOcProp->flOcclusionDirectRatio) - 1.0F;
		const float ratio_2 = pOcProp->flOcclusionLFRatio * pOcProp->flOcclusionDirectRatio;
		const float ratio = (ratio_2 > ratio_1) ? ratio_2 : ratio_1;

		pDirectLowPassFilter->flGain = MilliBelToLinearGain(occlusion_f * ratio);
		pDirectLowPassFilter->flGainHF = MilliBelToLinearGain(occlusion_f * pOcProp->flOcclusionDirectRatio);
	}


	// Room.
	//
	{
		const float ratio_1 = (pOcProp->flOcclusionLFRatio + pOcProp->flOcclusionRoomRatio) - 1.0F;
		const float ratio_2 = pOcProp->flOcclusionLFRatio * pOcProp->flOcclusionRoomRatio;
		const float ratio = (ratio_2 > ratio_1) ? ratio_2 : ratio_1;

		pSendLowPassFilter->flGain = MilliBelToLinearGain(occlusion_f * ratio);
		pSendLowPassFilter->flGainHF = MilliBelToLinearGain(occlusion_f * pOcProp->flOcclusionRoomRatio);
	}
}

void AdjustEnvironmentSize(
	EAXREVERBPROPERTIES* pEAXProp,
	float flEnvironmentSize)
{
	if (pEAXProp == NULL)
	{
		return;
	}

	flEnvironmentSize = oal_efx_util_clampf(
		flEnvironmentSize,
		EAXREVERB_MINENVIRONMENTSIZE,
		EAXREVERB_MAXENVIRONMENTSIZE
	);

	if (flEnvironmentSize == pEAXProp->flEnvironmentSize)
	{
		return;
	}

	const float scale = flEnvironmentSize / pEAXProp->flEnvironmentSize;

	pEAXProp->flEnvironmentSize = flEnvironmentSize;

	if ((pEAXProp->ulFlags & EAXREVERBFLAGS_DECAYTIMESCALE) != 0)
	{
		pEAXProp->flDecayTime = oal_efx_util_clampf(
			scale * pEAXProp->flDecayTime,
			EAXREVERB_MINDECAYTIME,
			EAXREVERB_MAXDECAYTIME
		);
	}

	if ((pEAXProp->ulFlags & EAXREVERBFLAGS_REFLECTIONSSCALE) != 0)
	{
		if ((pEAXProp->ulFlags & EAXREVERBFLAGS_REFLECTIONSDELAYSCALE) != 0)
		{
			pEAXProp->lReflections -= (long)(log10f(scale) * 2000.0F);

			pEAXProp->lReflections = oal_efx_util_clampl(
				pEAXProp->lReflections,
				EAXREVERB_MINREFLECTIONS,
				EAXREVERB_MAXREFLECTIONS
			);
		}
	}

	if ((pEAXProp->ulFlags & EAXREVERBFLAGS_REFLECTIONSDELAYSCALE) != 0)
	{
		pEAXProp->flReflectionsDelay = oal_efx_util_clampf(
			pEAXProp->flReflectionsDelay * scale,
			EAXREVERB_MINREFLECTIONSDELAY,
			EAXREVERB_MAXREFLECTIONSDELAY
		);
	}

	if ((pEAXProp->ulFlags & EAXREVERBFLAGS_REVERBSCALE) != 0)
	{
		const float log_scalar = ((pEAXProp->ulFlags & EAXREVERBFLAGS_DECAYTIMESCALE) != 0) ? 2000.0F : 3000.0F;

		pEAXProp->lReverb -= (long)(log10f(scale) * log_scalar);

		pEAXProp->lReverb = oal_efx_util_clampl(
			pEAXProp->lReverb,
			EAXREVERB_MINREVERB,
			EAXREVERB_MAXREVERB
		);
	}

	if ((pEAXProp->ulFlags & EAXREVERBFLAGS_REVERBDELAYSCALE) != 0)
	{
		pEAXProp->flReverbDelay = oal_efx_util_clampf(
			scale * pEAXProp->flReverbDelay,
			EAXREVERB_MINREVERBDELAY,
			EAXREVERB_MAXREVERBDELAY
		);
	}

	if ((pEAXProp->ulFlags & EAXREVERBFLAGS_ECHOTIMESCALE) != 0)
	{
		pEAXProp->flEchoTime = oal_efx_util_clampf(
			pEAXProp->flEchoTime * scale,
			EAXREVERB_MINECHOTIME,
			EAXREVERB_MAXECHOTIME
		);
	}

	if ((pEAXProp->ulFlags & EAXREVERBFLAGS_MODULATIONTIMESCALE) != 0)
	{
		pEAXProp->flModulationTime = oal_efx_util_clampf(
			scale * pEAXProp->flModulationTime,
			EAXREVERB_MINMODULATIONTIME,
			EAXREVERB_MAXMODULATIONTIME
		);
	}
}
