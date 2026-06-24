var WeatherProvider = require("./provider.js");

function request(url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
}

var OpenWeatherMapProvider = function (apiKey) {
  this._super.call(this);
  this.name = "OpenWeatherMap";
  this.id = "openweathermap";
  this.apiKey = apiKey;
  this.weatherDataCache = null;
};

OpenWeatherMapProvider.prototype = Object.create(WeatherProvider.prototype);
OpenWeatherMapProvider.prototype.constructor = OpenWeatherMapProvider;
OpenWeatherMapProvider.prototype._super = WeatherProvider;

OpenWeatherMapProvider.prototype.withOwmResponse = function (
  lat,
  lon,
  callback
) {
  var url =
    "https://api.openweathermap.org/data/3.0/onecall?appid=" +
    this.apiKey +
    "&lat=" +
    lat +
    "&lon=" +
    lon +
    "&units=imperial&exclude=alerts,minutely";

  request(
    url,
    "GET",
    function (response) {
      var weatherData = JSON.parse(response);
      console.log("Found timezone: " + weatherData.timezone);
      // cache weather data (use same request for sun events and weather forecast)
      this.weatherDataCache = weatherData;
      callback(weatherData);
    }.bind(this)
  );
};

OpenWeatherMapProvider.prototype.withWeatherData = function (
  lat,
  lon,
  callback
) {
  if (this.weatherDataCache === null) {
    this.withOwmResponse(lat, lon, function (owmResponse) {
      callback(owmResponse);
    });
  } else {
    callback(this.weatherDataCache);
  }
};

// ============== IMPORTANT OVERRIDE ================
OpenWeatherMapProvider.prototype.withSunEvents = function (lat, lon, callback) {
  console.log("This is the overridden implementation of withSunEvents");
  this.withOwmResponse(
    lat,
    lon,
    function (owmResponse) {
      var days = owmResponse.daily;
      var sunEvents = [
        { type: "sunrise", date: new Date(days[0].sunrise * 1000) },
        { type: "sunset", date: new Date(days[0].sunset * 1000) },
        { type: "sunrise", date: new Date(days[1].sunrise * 1000) },
        { type: "sunset", date: new Date(days[1].sunset * 1000) },
      ];
      var now = new Date();
      var nextSunEvents = sunEvents.filter(function (sunEvent) {
        return sunEvent.date > now;
      });
      var next24HourSunEvents = nextSunEvents.slice(0, 2);
      console.log(
        "The next " +
          sunEvents[0].type +
          " is at " +
          sunEvents[0].date.toTimeString()
      );
      console.log(
        "The next " +
          sunEvents[1].type +
          " is at " +
          sunEvents[1].date.toTimeString()
      );
      callback(next24HourSunEvents);
    }.bind(this)
  );
};

OpenWeatherMapProvider.prototype.withProviderData = function (
  lat,
  lon,
  force,
  callback
) {
  // callBack expects that this.hasValidData() will be true
  console.log("This is the overridden implementation of withProviderData");
  this.withWeatherData(
    lat,
    lon,
    function (weatherData) {
      this.tempTrend = weatherData.hourly.map(function (entry) {
        return entry.temp;
      });
      this.precipTrend = weatherData.hourly.map(function (entry) {
        return entry.pop;
      });
      this.precipMMH = weatherData.hourly.map(function (entry) {
        return entry.rain ? entry.rain["1h"] : 0;
      });
      this.windSpeed = weatherData.hourly.map(function (entry) {
        return entry.wind_speed;
      });
      this.daysTemp = weatherData.daily.map(function (entry) {
        return entry.temp.max;
      });
      this.daysPop = weatherData.daily.map(function (entry) {
        return entry.pop;
      });
      // Compute 3-period precipitation (morning, midday, afternoon) per day
      var dailyData = weatherData.daily;
      var hourlyData = weatherData.hourly;
      this.daysPopPeriods = [];
      for (var d = 0; d < 7; d++) {
        var dayStart = dailyData[d].dt;
        var dayEnd = dayStart + 86400;
        var periods = [[], [], []];
        for (var h = 0; h < hourlyData.length; h++) {
          var hr = hourlyData[h];
          if (hr.dt >= dayStart && hr.dt < dayEnd) {
            var hourOfDay = new Date(hr.dt * 1000).getUTCHours();
            var periodIdx = hourOfDay < 8 ? 0 : hourOfDay < 16 ? 1 : 2;
            periods[periodIdx].push(hr.pop);
          }
        }
        for (var p = 0; p < 3; p++) {
          if (periods[p].length > 0) {
            var max = Math.max.apply(null, periods[p]);
            this.daysPopPeriods.push(Math.round(max * 100));
          } else {
            this.daysPopPeriods.push(Math.round(dailyData[d].pop * 100));
          }
        }
      }
      this.daysMMH = weatherData.daily.map(function (entry) {
        return entry.rain ? entry.rain : 0;
      });
      this.daysIcon = weatherData.daily.map(function (entry) {
        return entry.weather[0].id;
      });
      this.startTime = weatherData.hourly[0].dt;
      this.currentTemp = weatherData.current.temp;
      this.uvi = weatherData.current.uvi;
      callback();
    }.bind(this)
  );
};

module.exports = OpenWeatherMapProvider;
