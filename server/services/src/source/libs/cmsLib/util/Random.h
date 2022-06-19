#ifndef __RANDOM_H__
#define __RANDOM_H__

#include <cmsLib/GlobalSettings.h>
#include <stdlib.h>

//[0, 0xffffffff]
inline int CMS_Rand()
{
    int randtemp = rand();
    return randtemp;
}

//[0, rmax)
inline int CMS_RandMax(int rmax)
{
    return (rmax > 0) ? (CMS_Rand() % rmax) : 0;
}

//[rmin, rmax)
inline int CMS_RandMinMax(int rmin, int rmax)
{
    return (rmin < rmax) ? (CMS_Rand() % (rmax-rmin) + rmin) : rmin;
}

//[0, 99]
inline int CMS_Rand100()
{
    return CMS_RandMax(100);
}

//[0, 9999]
inline int CMS_Rand10000()
{
    return CMS_RandMax(10000);
}

inline int CMS_RandMinMaxTwoLoop(int rmin, int rmax)
{
    return (CMS_RandMinMax(rmin, rmax) + CMS_RandMinMax(rmin, rmax)) / 2;
}

// chance皆为百分比
inline bool Rand(float chance)
{
    int val = CMS_RandMax(10000);
    int p = int(chance * 100.0f);
    return p > val;
}

inline bool Rand(unsigned int chance)
{
    int val = CMS_RandMax(10000);
    int p = int(chance * 100);
    return p > val;
}

inline bool Rand(int chance)
{
    int val = CMS_RandMax(10000);
    int p = chance * 100;
    return p > val;
}

inline bool Rand10000(int chance)
{
    int val = CMS_RandMax(10000);
    return chance > val;
}

#endif // __RANDOM_H__