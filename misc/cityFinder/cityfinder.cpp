#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <map>

// We haven't seen these header files yet, but we will next lecture.
#include <algorithm>
#include <iterator>

using namespace std;

// CityLocation stores latitude and longitude coordinates for a city.
struct CityLocation {
  double latitude;
  double longitude;
};

// We use typedef to create type "nicknames" so that we don't have to type out
// map<string, CityLocation> and multimap<double, string> every time. Be sure to
// pick informative names!
using CityMap = map<string, CityLocation>;
using CoordMap = multimap<double, string>;

// If your compiler doesn't support using, comment the above lines and uncomment
// the typedefs below:
// template map<string, CityLocation> CityMap;
// template multimap<double, string> CoordMap;



// Rough conversion from miles to latitude/longitude degrees. This works pretty
// well for the United States, though it gets a little wonky near Alaska.
const double kDegreesPerMile = 1.0 / 69.172;

// kPlaceDataFile is the name of the file that contains the location information
// for all of the cities.
const string kPlaceDataFile = "place-data.txt";

// GetLine waits for the user to type something in at the console, and returns
// all of it as a string.
string GetLine() {
  string result;
  getline(cin, result);
  return result;
}

// GetPositiveReal prompts the user to enter a real number that's positive. If
// the input is invalid, the function loops.
double GetPositiveReal()
{
  while (true)
  {
    stringstream converter;
    converter << GetLine();
    
    double result;
    converter >> result;
    if (converter.fail())
    {
      cout << "Please enter a real number." << endl;
    } 
    else
    {
      char junk;
      converter >> junk;
      if (converter.fail() && result > 0.0)
      {
        return result;
      } 
      else if (converter.fail())
      {
        cout << "Please enter a positive real number." << endl;
      }
      else
      {
        cout << "Unexpected character: " << junk << endl;
      }
    }
    cout << "Retry: ";
  }
}

// Distance returns the Euclidean distance between the two given cities.
double Distance(CityLocation lhs, CityLocation rhs) {
  double dLat = lhs.latitude - rhs.latitude;
  double dLong = lhs.longitude - rhs.longitude;
  return sqrt(dLat * dLat + dLong * dLong);
}

// LoadData reads in the file containing city location information and populates
// the city lookup map with that data.
void LoadData(CityMap& cityLookup)
{
  ifstream input(kPlaceDataFile);
  string data, name;

  // We call getline twice, since the information for a city is split across two
  // lines -- the name first, then the latitude/longitude coordinates.
  while (getline(input, name))
  {
    CityLocation location;
    input >> location.latitude >> location.longitude >> ws;

    // The following syntax is commonly used to insert values into a map. The
    // alternative would be to use:
    //
    //   cityLookup[name] = location;
    //
    // The [] notation doesn't work for multimaps, as we'll see later. It's also
    // slightly less efficient, although for most applications the difference is
    // trivial.
    cityLookup.insert(make_pair(name, location));
  }

  cout << "Data exists for " << cityLookup.size() << " cities." << endl;
}

// PopulateLookupTables takes the city lookup map that was populated by LoadData
// and inserts the data into the reverse lookup maps for latitude and longitude.
// These maps store the coordinate as the key and the name of the city as the
// value.
void PopulateLookupTables(CityMap& cityLookup, CoordMap& latLookup, CoordMap& longLookup)
{
  // Iterate through every city we processed:
  for (auto& pair : cityLookup)
  {
    // We insert using the make_pair notation. The insert function expects a
    // pair<double, string> object as its argument (i.e. pair with key and value
    // as its two fields), which we can construct using make_pair.
    latLookup.insert(make_pair(pair.second.latitude, pair.first));
    longLookup.insert(make_pair(pair.second.longitude, pair.first));
  }
}

// FindNearbyCities processes the given city and radius and locates all of the
// cities within range.
void FindNearbyCities(CityMap& cityLookup, CoordMap& latLookup, CoordMap& longLookup, double radius, CityLocation center)
{
  // nearbyLats will store the names of all cities whose latitude coordinates
  // are within range. The minimum possible latitude coordinate is radius less
  // than the center's latitude, while the maximum possible latitude is radius
  // more than the center's latitude.
  set<string> nearbyLats;
  for (auto itr = latLookup.lower_bound(center.latitude - radius); itr != latLookup.upper_bound(center.latitude + radius); ++itr)
    nearbyLats.insert(itr->second);

  // We repeat the above approach, but with candidate cities according to
  // longitude.
  set<string> nearbyLongs;
  for (auto itr = longLookup.lower_bound(center.longitude - radius); itr != longLookup.upper_bound(center.longitude + radius); ++itr)
    nearbyLongs.insert(itr->second);

  // We need to find the intersection of the two sets, nearbyLats and
  // nearbyLongs, so that we know which cities we need to test. The function
  // set_intersection is an STL algorithm, which we will cover on Thursday.
  // Don't worry about understanding the syntax here; the net result is that
  // the candidates vector will store the result of the intersection of the two
  // sets.
  vector<string> candidates;
  set_intersection(nearbyLats.begin(), nearbyLats.end(),
                   nearbyLongs.begin(), nearbyLongs.end(),
                   back_inserter(candidates));

  // For each of the candidate cities, check the actual distance and verify that
  // it is within the radius.
  for (const string& cityName : candidates)
  {
    CityLocation city = cityLookup[cityName];
    if (Distance(city, center) < radius)
      cout << cityName << endl;
  }
}

int main()
{
  // cityLookup is a mapping from city names to their locations.
  CityMap cityLookup;
  LoadData(cityLookup);

  // latLookup and longLookup are maps from a latitude/longitude coordinate to
  // the name of the city at that coordinate. Since we might have multiple
  // cities with the same latitude or longitude coordinate, we need to store
  // this data in multimap objects. They behave just like STL maps, except that
  // they can store multiple values for each key.
  //
  // The way it does so is by storing multiple key-value pairs with the same
  // key, each one having a different value. We use iterators to process all of
  // them.
  CoordMap latLookup;
  CoordMap longLookup;
  PopulateLookupTables(cityLookup, latLookup, longLookup);

  while (true)
  {
    cout << "Please enter a city: ";
    string name = GetLine();
    if (name.empty()) break;

    // We first need to check if the city exists in our map. We perform this
    // check on cityLookup by calling .find(...). If that city name is a key in
    // the map, the iterator will be pointing at the key-value pair with that
    // particular key. Otherwise, the iterator will be equal to
    // cityLookup.end(), which simply happens to be a convenient sentinel value
    // as it doesn't actually point to anything in the map.
    auto cityItr = cityLookup.find(name);
    if (cityItr == cityLookup.end())
    {
      cout << "No!" << endl;
    } 
    else
    {
      cout << "Enter a distance (miles): ";
      double radius = GetPositiveReal() * kDegreesPerMile;
      FindNearbyCities(cityLookup, latLookup, longLookup, radius, cityItr->second);
    }

    cout << endl;
  }

  return 0;
}
