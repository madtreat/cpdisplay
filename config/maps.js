var view;
var satLayer;
var hybLayer;
var osmLayer;
var map;

var acIDs = [];
var acFeatures = [];
var acIconLayers = []; // one icon per aircraft
var acElements = []; // one element per aircraft
var acPopupOverlays = []; // one popup overlay per aircraft

function getCoords(lat, lon) {
   return ol.proj.transform([lon, lat], 'EPSG:4326', 'EPSG:3857');
}

function init() {
   view = new ol.View({
      center: getCoords(__LAT__, __LON__),
      zoom: __ZOOM__
   });
   
   satLayer = new ol.layer.Tile({
      source: new ol.source.MapQuest({layer: 'sat'})
   });
   hybLayer = new ol.layer.Tile({
      source: new ol.source.MapQuest({layer: 'hyb'})
   });
   osmLayer = new ol.layer.Tile({
      source: new ol.source.MapQuest({layer: 'osm'}),
      visible: false
   });
   
   map = new ol.Map({
      target: 'map',
      layers: [satLayer, hybLayer, osmLayer],
      allOverlays: true,
      view: view,
      interactions: [],
      controls: []
   });
   
   map.on('click', function(evt) {
      var feature = map.forEachFeatureAtPixel(evt.pixel,
      function(feature, layer) {
         return feature;
      });
      if (feature) {
         var id = feature.get('id') - 1; // zero based, but ac 0 is not shown, so -1
         var geometry = feature.getGeometry();
         var coord = geometry.getCoordinates();
         acPopupOverlays[id].setPosition(coord);
         var element = acElements[id];
         var content = 'id : '+ feature.get('id') + '\n';
         content += 'alt: ' + feature.get('alt') + '\n';
         content += 'rng: ' + feature.get('rng') + '\n';
         content += 'ber: ' + feature.get('ber');
         $(element).popover({
            'placement': 'right',
            'html': true,
            'content': content
         });
         $(element).popover('show');
      } else {
         $(acElements).each(function(index) {
            $(this).popover('destroy');
         });
      }
   });
   
   // change mouse cursor when over marker
   map.on('pointermove', function(e) {
      if (e.dragging) {
         $(acElements).each(function(index) {
            $(this).popover('destroy');
         });
         return;
      }
      var pixel = map.getEventPixel(e.originalEvent);
      var hit = map.hasFeatureAtPixel(pixel);
      map.getTarget().style.cursor = hit ? 'pointer' : '';
   });
}

function resize() {
   setTimeout( function() { map.updateSize(); }, 200);
}

function panTo(lat, lon) {
   view.setCenter(getCoords(lat, lon));
}

function zoomTo(level) {
   view.setZoom(level);
}

/*
 * Rotates the map around the provided lat/lon coords by deg degrees.
 * 
 * @param {type} deg
 * @param {type} lat
 * @param {type} lon
 */
function rotate(deg, lat, lon) {
   view.rotate(deg*Math.PI/180, getCoords(lat, lon));
}

/*
 * Makes the satellite layer invisible and shows the street map.
 */
function removeSatLayer() {
   satLayer.set('visible', false);
   hybLayer.set('visible', false);
   osmLayer.set('visible', true);
}

/*
 * Makes the satellite layer visible with a hybrid layer on top to include
 * all major map features (countries, states, cities, roads, airports, etc.),
 * and hides the street map.
 */
function addSatLayer() {
   osmLayer.set('visible', false);
   satLayer.set('visible', true);
   hybLayer.set('visible', true);
}

/*
 * Updates the aircraft's feature data, which is displayed in the popup when
 * clicking or tapping on an aircraft.
 * 
 * @param {type} id  - aircraft ic
 * @param {type} lat - latitude
 * @param {type} lon - longitude
 * @param {type} rng - distance to aircraft
 * @param {type} ber - bearing to aircraft
 * @param {type} alt - aircraft altitude
 * @param {type} hdg - aircraft heading (in degrees)
 */
function updateAircraft(id, lat, lon, rng, ber, alt, hdg) {
   var feature = acFeatures[id-1];
   if (feature) {
      feature.setGeometry(new ol.geom.Point(getCoords(lat, lon)));
      feature.set('id', id);
      feature.set('rng', rng);
      feature.set('ber', ber);
      feature.set('alt', alt);
      feature.set('hdg', hdg);
      
      acIconLayers[id-1].refresh({force:true});
   }
}

/*
 * Draws an aircraft as an icon VectorLayer on the map.
 * 
 * @param {type} id  - aircraft ic
 * @param {type} lat - latitude
 * @param {type} lon - longitude
 * @param {type} rng - distance to aircraft
 * @param {type} ber - bearing to aircraft
 * @param {type} alt - aircraft altitude
 * @param {type} hdg - aircraft heading (in degrees)
 */
function addNewAircraft(id, lat, lon, rng, ber, alt, hdg) {
   if (acIDs.indexOf(id) >= 0) {
      updateAircraft(id, lat, lon, rng, ber, alt, hdg);
   }
   else {
      acIDs.push(id);
      var iconFeature = new ol.Feature({
         geometry: new ol.geom.Point(getCoords(lat, lon)),
         id: id,
         rng: rng,
         ber: ber,
         alt: alt,
         hdg: hdg
      });
      var iconStyle = new ol.style.Style({
         image: new ol.style.Icon(/** @type {olx.style.IconOptions} */ ({
            anchor: [0.5, 24],
            anchorXUnits: 'fraction',
            anchorYUnits: 'pixels',
            opacity: 0.75,
            rotation: hdg * Math.pi/180,
            src: '../src/resources/icons/airplane.png'
         }))
      });
      iconFeature.setStyle(iconStyle);
      acFeatures.push(iconFeature);
      
      var vectorSource = new ol.source.Vector({
         features: [iconFeature]
      });
      var vectorLayer = new ol.layer.Vector({
         source: vectorSource
      });
      map.addLayer(vectorLayer);
      acIconLayers.push(vectorLayer); // so we can access it later
      
      var element = document.getElementById('popup' + id);
      acElements.push(element);
      var popup = new ol.Overlay({
         element: element,
         positioning: 'bottom-center',
         stopEvent: false
      });
      map.addOverlay(popup);
      acPopupOverlays.push(popup);
   }
}