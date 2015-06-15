// This example creates circles on the map, representing
// populations in North America.

// First, create an object containing LatLng and population for each city.
var citymap = {};
citymap['collegestation'] = {
   center: new google.maps.LatLng(30.637778, -96.478611),
   population: 271456
};
//citymap['chicago'] = {
//  center: new google.maps.LatLng(41.878113, -87.629798),
//  population: 2714856
//};
//citymap['newyork'] = {
//  center: new google.maps.LatLng(40.714352, -74.005973),
//  population: 8405837
//};
//citymap['losangeles'] = {
//  center: new google.maps.LatLng(34.052234, -118.243684),
//  population: 3857799
//};
//citymap['vancouver'] = {
//  center: new google.maps.LatLng(49.25, -123.1),
//  population: 603502
//};

var cityCircle;

function initialize() {
   // Create the map.
   var mapOptions = {
      center: new google.maps.LatLng(__LAT__, __LON__),
      zoom: __ZOOM__,
      mapTypeId: google.maps.MapTypeId.__MAP_TYPE__,
      panControl: true
   };
   
   var map = new google.maps.Map(document.getElementById('map-canvas'), mapOptions);
   
   // Construct the circle for each value in citymap.
   // Note: We scale the area of the circle based on the population.
//   for (var city in citymap) {
//      var populationOptions = {
//         strokeColor: '#FF0000',
//         strokeOpacity: 0.8,
//         strokeWeight: 2,
//         fillColor: '#FF0000',
//         fillOpacity: 0.35,
//         map: map,
//         center: citymap[city].center,
//         radius: Math.sqrt(citymap[city].population) * 100
//      };
//      // Add the circle for this city to the map.
//      cityCircle = new google.maps.Circle(populationOptions);
//   }

   var populationOptions = {
      strokeColor: '#FF0000',
      strokeOpacity: 0.8,
      strokeWeight: 2,
      fillColor: '#FF0000',
      fillOpacity: 0.35,
      map: map,
      center: new google.maps.LatLng(30.637778, -96.478611),
      radius: 50
   };
   // Add the circle for this city to the map.
   cityCircle = new google.maps.Circle(populationOptions);
}

google.maps.event.addDomListener(window, 'load', initialize);