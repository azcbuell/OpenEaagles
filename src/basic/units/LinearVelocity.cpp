
#include "openeaagles/basic/units/LinearVelocity.h"
#include "openeaagles/basic/units/Distances.h"
#include "openeaagles/basic/units/Times.h"
#include "openeaagles/basic/SlotTable.h"

namespace Eaagles {
namespace Basic {

//---------------------------------------------------------------------
// Conversion Constants:
//---------------------------------------------------------------------
const double LinearVelocity::FPS2KTSCC = 0.59248384864341771016513018598233;

//---------------------------------------------------------------------
// LinearVelocity
//---------------------------------------------------------------------
IMPLEMENT_SUBCLASS(LinearVelocity, "LinearVelocity")
EMPTY_SERIALIZER(LinearVelocity)

//---------------------------------------------------------------------
// Slot Table:
//---------------------------------------------------------------------
BEGIN_SLOTTABLE(LinearVelocity)
    "distance",    // 1: distance 
    "time",        // 2: time
END_SLOTTABLE(LinearVelocity)

// Map slot table to handles 
BEGIN_SLOT_MAP(LinearVelocity)
    ON_SLOT(1, setSlotDistance, Distance)
    ON_SLOT(2, setSlotTime, Time)
END_SLOT_MAP()

//---------------------------------------------------------------------
// Constructors:
//---------------------------------------------------------------------

LinearVelocity::LinearVelocity()
{
    STANDARD_CONSTRUCTOR()
    
    //Set a default distance, time, and linearVelocity
    distance = 1;
    time = 1;
    val = 1;
}

LinearVelocity::LinearVelocity(const LCreal newLinearVelocityMetersPerSec)
{
    STANDARD_CONSTRUCTOR()
    
    //Set the distance to the input and linearVelocity to the input and make seconds 1 to get meters per second:
    distance = newLinearVelocityMetersPerSec;
    time = 1;
    val = newLinearVelocityMetersPerSec;
}

LinearVelocity::LinearVelocity(const Distance* const newDistance, const Time* const newTime)
{
    STANDARD_CONSTRUCTOR()

    //Set Checks to false:
    bool okDistance = false;
    bool okTime = false;

    //Check and convert the distance to meters
    if (newDistance != 0)
    {
        LCreal finalDistance = Meters::convertStatic( *newDistance );
        okDistance = setMeters(finalDistance);
    }

    //Check and convert the time to seconds
    if (newTime != 0)
    {
        LCreal finaltime = Seconds::convertStatic( *newTime );
        okTime = setSeconds(finaltime);
    }
    
    //Check that both were set correctly - if not give error:
    if ( !okTime || !okDistance )
    {
        //Give error if something was not set correctly:
        std::cerr << "Distance or Time not set correctly - new LinearVelocity Object bad." << std::endl;

    }

}
//---------------------------------------------------------------------
// getSlotByIndex() for LinearVelocity
//---------------------------------------------------------------------
Object* LinearVelocity::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//---------------------------------------------------------------------
// copyData() -- copy this object's data:
//---------------------------------------------------------------------
void LinearVelocity::copyData(const LinearVelocity& org, const bool)
{
    BaseClass::copyData(org);

    distance = org.distance;
    time = org.time;

}

//---------------------------------------------------------------------
// deleteData() -- delete this object's data:
//---------------------------------------------------------------------
void LinearVelocity::deleteData()
{
}

//---------------------------------------------------------------------
// getLinearVelocity() -- returns Linear Velocity:
//---------------------------------------------------------------------
LCreal LinearVelocity::getMetersPerSecond() const
{
    return (LCreal)(val); 
};

//---------------------------------------------------------------------
// convert() -- converts from meters/sec to desired distance/time ratio:
//    NOTE: This ignores values of input - only the object type is used
//---------------------------------------------------------------------
LCreal LinearVelocity::convert(Distance* newDistanceUnit, Time* newTimeUnit)
{

    //Init a num to -1 as a check:
    LCreal desiredDistance = -1.0f;
    LCreal desiredTime = -1.0f;
    LCreal desiredResult = -1.0f;
    
    //Set input object's internal value to 1 as a precaution:
    newDistanceUnit->setValue(1);
    newTimeUnit->setValue(1);

    //Take the internal unit and create an object of Meters to convert distances:
    Meters* internalMeters = new Meters((LCreal)distance);

    //Find out what units the distance is in:
    if(dynamic_cast<Feet*>(newDistanceUnit) != 0)
    {
        //New distance is in Feet:
        Feet* feet = new Feet;
        desiredDistance = feet->convert(*internalMeters);
        feet->unref();
    }
    else if(dynamic_cast<Meters*>(newDistanceUnit) != 0)
    {
        //New distance is in Meters:
        Meters* meters = new Meters;
        desiredDistance = meters->convert(*internalMeters);
        meters->unref();
    }
    else if(dynamic_cast<CentiMeters*>(newDistanceUnit) != 0)
    {
        //New distance is in CentiMeters:
        CentiMeters* centimeters = new CentiMeters;
        desiredDistance = centimeters->convert(*internalMeters);
        centimeters->unref();
    }
    else if(dynamic_cast<KiloMeters*>(newDistanceUnit) != 0)
    {
        //New distance is in KiloMeters:
        KiloMeters* kilometers = new KiloMeters;
        desiredDistance = kilometers->convert(*internalMeters);
        kilometers->unref();
    }
    else if(dynamic_cast<Inches*>(newDistanceUnit) != 0)
    {
        //New distance is in Inches:
        Inches* inches = new Inches;
        desiredDistance = inches->convert(*internalMeters);
        inches->unref();
    }
    else if(dynamic_cast<NauticalMiles*>(newDistanceUnit) != 0)
    {
        //New distance is in NauticalMiles:
        NauticalMiles* nauticalmiles = new NauticalMiles;
        desiredDistance = nauticalmiles->convert(*internalMeters);
        nauticalmiles->unref();
    }
    else if(dynamic_cast<StatuteMiles*>(newDistanceUnit) != 0)
    {
        //New distance is in StatuteMiles:
        StatuteMiles* statutemiles = new StatuteMiles;
        desiredDistance = statutemiles->convert(*internalMeters);
        statutemiles->unref();
    }
    else
    {
        //Give Error - Not sure what type it is:
        std::cerr << "Distance Conversion Type Not Found." << std::endl;
    }
    internalMeters->unref();

    //Find out what units the time input is in - do not use built in convert - very easy to do by hand:
    Seconds* q = dynamic_cast<Seconds*>(newTimeUnit);
    if(q != 0)
    {
        desiredTime = time;
    }
    else if(dynamic_cast<MilliSeconds*>(newTimeUnit) != 0)
    {
        //Time in milliseconds:
        desiredTime = time*1000;
    }
    else if(dynamic_cast<Minutes*>(newTimeUnit) != 0)
    {
        //Time in minutes:
        desiredTime = time/60;
    }
    else if(dynamic_cast<Hours*>(newTimeUnit) != 0)
    {
        //Time in hours:
        desiredTime = time/3600;
    }
    else if(dynamic_cast<Days*>(newTimeUnit) != 0)
    {
        //Time in days:
        desiredTime = time/86400;
    }
    else
    {
        //Give Error - Not sure what type it is:
        std::cerr << "Time Conversion Type Not Found." << std::endl;
    };
    
    desiredResult = desiredDistance/desiredTime;

    return desiredResult;

}
    
//---------------------------------------------------------------------
// setMetersPerSecond() -- sets our linearVelocity:
//---------------------------------------------------------------------
bool LinearVelocity::setMetersPerSecond(const LCreal newLinearVelocity)
{

    //Set distance and time - units in meters per second -> num = input; den = 1:
    bool ok1 = setMeters(newLinearVelocity);
    bool ok2 = setSeconds(1);

    //Check both values for ok:
    ok1 = (ok1)&&(ok2);
            
    return ok1;
}    

//---------------------------------------------------------------------
// setSlotDistance() -- sets distance based on input object and its value:
//---------------------------------------------------------------------
bool LinearVelocity::setSlotDistance(const Distance* const msg)
{
    bool ok = false;

    //Try to convert Number to an distance:
    if(msg != 0)
    {
        LCreal finalNumber = Meters::convertStatic(*msg);
        ok = setMeters(finalNumber);
    }
    return ok;
}

//---------------------------------------------------------------------
// setSlotTime() -- sets time based on input object and its value:
///---------------------------------------------------------------------
bool LinearVelocity::setSlotTime(const Time* const msg)
{
    bool ok = false;

    //Try to convert Number to an distance:
    if(msg != 0)
    {
        LCreal finalNumber = Seconds::convertStatic(*msg);
        ok = setSeconds(finalNumber);
    }
    return ok;
}

//---------------------------------------------------------------------
// setMeters() -- sets distance of object in (meters):
//---------------------------------------------------------------------
bool LinearVelocity::setMeters(const LCreal newDistance)
{
    distance = newDistance;
    //Update new linear velocity:
    val = distance/time;
    return true;
}

//---------------------------------------------------------------------
// setTime() -- sets time of object in (seconds):
//---------------------------------------------------------------------
bool LinearVelocity::setSeconds(const LCreal newTime)
{
    //Set Time:
    time = newTime;
    //Update new linear velocity:
    val = distance/time;
    return true;
}


} // End Basic namespace
} // End Eaagles namespace
