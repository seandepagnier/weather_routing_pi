class ChargeSource
{
  double 
};

class SolarArraySource : public ChargeSource
{
  double arraysurface_area;
  double efficiency;
  double ComputeCloudCoverPercentage();
  double CurrentAveragePower(wxTimeSpan span);
};

class WindGeneratorSource : public ChargeSource
{
};

class AirXMarine : public ChargeSource
{
};

/* can apply drag to generate electricity at the cost of boat speed */
class HydroGeneratorSource : public ChargeSource
{

/* a fuel powered generator is assumed to be able
   to produce a given amount of power and supply
   it to charge batteries.  */
class Generator : public ChargeSource
{
  double kwh;
};
