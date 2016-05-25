#include "common.h"

inline fp1v
max2(fp1v x, fp1v y)
{
  return x > y ? x : y;
}

inline fp1v
max3(fp1v x, fp1v y, fp1v z)
{
  return max2(max2(x, y), z);
}

inline fp1v
max4(fp1v x, fp1v y, fp1v z, fp1v w)
{
  return max2(max3(x, y, z), w);
};

inline fp1v
min2(fp1v x, fp1v y)
{
  return x > y ? y : x;
}

inline fp1v
min3(fp1v x, fp1v y, fp1v z)
{
  return min2(min2(x, y), z);
}

inline fp1v
min4(fp1v x, fp1v y, fp1v z, fp1v w)
{
  return min2(min3(x, y, z), w);
}
