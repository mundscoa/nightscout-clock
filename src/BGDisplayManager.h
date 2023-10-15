#ifndef BGDisplayManager_h
#define BGDisplayManager_h

#include "NightscoutManager.h"
#include "BGDisplayFace.h"
#include "BGDisplayFaceSimple.h"
#include "BGDisplayFaceGraph.h"

#include <Arduino.h>
#include <list>
#include <vector>

struct GlucoseInterval
{
    int low_boundary;
    int high_boundary;
    BG_LEVEL intarval_type;
};

struct GlucoseIntervals
{
    std::vector<GlucoseInterval> intervals; // Use a vector to store the intervals

    // Method to add a GlucoseInterval to the array
    void addInterval(int low, int high, BG_LEVEL type)
    {
        intervals.push_back({low, high, type});
    }

    BG_LEVEL getBGLevel(int value)
    {
        for (const GlucoseInterval &interval : intervals)
        {
            if (value >= interval.low_boundary && value <= interval.high_boundary)
            {
                return interval.intarval_type;
            }
        }
        return INVALID; // Default to INVALID if not found in any interval
    }

    String toString() const
    {
        String ss = "ColorIntervals:\n";
        for (const GlucoseInterval &interval : intervals)
        {
            ss += "  Low: " + String(interval.low_boundary) + ", High: " + String(interval.high_boundary) + ", Level: ";
            switch (interval.intarval_type)
            {
            case URGENT_HIGH:
                ss += "URGENT_HIGH";
                break;
            case WARNING_HIGH:
                ss += "WARNING_HIGH";
                break;
            case NORMAL:
                ss += "NORMAL";
                break;
            case WARNING_LOW:
                ss += "WARNING_LOW";
                break;
            case URGENT_LOW:
                ss += "URGENT_LOW";
                break;
            case INVALID:
                ss += "INVALID";
                break;
            }
        }
        return ss;
    }
};

class BGDisplayManager_
{
private:
    std::list<GlucoseReading> displayedReadings;
    std::vector<BGDisplayFace *> faces;
    BGDisplayFace *currentFace;
    int currentFaceIndex;
    GlucoseIntervals glucoseIntervals;

public:
    static BGDisplayManager_ &getInstance();
    void setup();
    void tick();
    void showData(std::list<GlucoseReading> glucoseReadings);
    unsigned long long getLastDisplayedGlucoseEpoch();
    GlucoseIntervals getGlucoseIntervals();

    void setFace(int id);
};

extern BGDisplayManager_ &BGDisplayManager;

#endif