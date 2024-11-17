// animate on scroll
AOS.init();

// state
let currentLocation = "Massachusetts";
let units = "metric";

// current weather selector
let displayLocation = document.querySelector(".location");
let datetime = document.querySelector(".datetime");
let weatherStatus = document.querySelector(".weather-status");
let temperature = document.querySelector(".temperature");
let weatherIcon = document.querySelector(".weather-icon");
let minMax = document.querySelector(".minmax-temp");
let feel = document.querySelector(".feel");
let airQuality = document.querySelector(".air-quality");
let humidity = document.querySelector(".humidity");
let windSpeed = document.querySelector(".wind-speed");
let pressure = document.querySelector(".pressure");
let clouds = document.querySelector(".clouds");
let sunrise = document.querySelector(".sunrise-time");
let sunset = document.querySelector(".sunset-time");

// Initialize an array to store elements
// next 48 hours selectors
let next48Times = [];
let next48Icons = [];
let nect48Temps = [];
// 5-days forecast selectors
let dateDay = [null];
let iconDay = [null];
let tempDay = [null];
let statDay = [null];

// Iterate over next-48 hours selectors
for (let i = 1; i <= 16; i++) {
    // Create a selector for the corresponding class
    let timeClass = `.next-48-time-${i}`;
    let iconClass = `.next-48-icon-${i}`;
    let tempClass = `.next-48-temp-${i}`;
    
    // Retrieve elements from the DOM
    let next48Time = document.querySelector(timeClass);
    let next48Icon = document.querySelector(iconClass);
    let next48Temp = document.querySelector(tempClass);
    
    // Store elements into an array
    next48Times.push(next48Time);
    next48Icons.push(next48Icon);
    nect48Temps.push(next48Temp);

    if (i == 1) {
      // Iterate over 5-days forecast selectors
      for (let j = 1; j <= 5; j++) {
        // Create a selector for the corresponding class
        let dateDayClass = `.dt-day-${j}`;
        let iconDayClass = `.icon-day-${j}`;
        let tempDayClass = `.temp-day-${j}`;
        let statDayClass = `.stat-day-${j}`;

        // Retrieve elements from the DOM
        let dateDays = document.querySelector(dateDayClass);
        let iconDays = document.querySelector(iconDayClass);
        let tempDays = document.querySelector(tempDayClass);
        let statDays = document.querySelector(statDayClass);

        // Store elements into an array
        dateDay.push(dateDays);
        iconDay.push(iconDays);
        tempDay.push(tempDays);
        statDay.push(statDays);
      }
    }
}


// search
// const searchBtn = document.querySelector(".btn-search");
const searchForm = document.querySelector(".location-form");
let search = document.querySelector(".get-location");

document.querySelector("#btn-search").addEventListener("click", function() {
  search.classList.toggle("show");
  searchForm.classList.toggle("show-border");
  document.querySelector(".search-icon").classList.toggle("show");
});

searchForm.addEventListener("submit", (e) => {
  // prevent default action
  e.preventDefault();
  // change current location
  currentLocation = search.value;
  // get weather forecast
  getWeather();
  // clear form
  search.value = "";
});

let search2 = document.querySelector(".get-location-2");
document.querySelector(".location-form-2").addEventListener("submit", (e) => {
  // prevent default action
  e.preventDefault();
  // change current location
  currentLocation = search2.value;
  // get weather forecast
  getWeather();
  // clear form
  search2.value = "";
});


// units
let switchUnits = document.querySelector(".switch-units");

switchUnits.addEventListener('click', () => { // switch units
  if (units !== "metric") {
    // change to metric
    units = "metric";
    switchUnits.innerHTML = "&#176C";
    // get weather forecast
    getWeather();
  } else if (units !== "imperial") {
    // change to imperial
    units = "imperial";
    switchUnits.innerHTML = "&#176F";
    // get weather forecast
    getWeather();
  }
});


function convertKelvin(unit, kelvin) {
  let result;

  if (unit == "metric") {
    result = kelvin - 273.15;
  } else if (unit == "imperial") {
    result = (kelvin - 273.15) * (9 / 5) + 32;
  }

  return result.toFixed(0);
};


function getFormatDate(timestamp) {
  
  const date = new Date(timestamp * 1000);

  const dayName = date.toLocaleString("en-US", { weekday: "long" }).substring(0, 3);
  const monthName = date.toLocaleString("en-US", { month: "long" }).substring(0, 3);
  const dayNumber = date.getDate();
  // const yearNumber = date.getFullYear(); //year

  return `${dayName}, ${monthName} ${dayNumber}`
}


function getFormatTime(timestamp, timezone){
  const convertTimezone = timezone / 3600; // convert seconds to hours 

  const date = new Date(timestamp * 1000);

  const options = {
    hour: "numeric",
    minute: "numeric",
    timeZone: `Etc/GMT${convertTimezone >= 0 ? "-" : "+"}${Math.abs(convertTimezone)}`,
    hour12: true,
  };

  return date.toLocaleString("en-US", options);
}


function getFormatHour(timestamp, timezone){
  const convertTimezone = timezone / 3600; // convert seconds to hours 

  const date = new Date(timestamp * 1000);

  const options = {
    hour: "numeric",
    timeZone: `Etc/GMT${convertTimezone >= 0 ? "-" : "+"}${Math.abs(convertTimezone)}`,
    hour12: true,
  };

  return date.toLocaleString("en-US", options);
}


function getAirQuality(airQualityIndex) {
  let airQuality;
  let airQualityIndexColor;

  switch (airQualityIndex) {
    case 1: 
      airQuality = "Good";
      airQualityIndexColor = "text-green-500";
      break;
    case 2:
      airQuality = "Fair";
      airQualityIndexColor = "text-yellow-500";
      break;
    case 3:
      airQuality = "Moderate";
      airQualityIndexColor = "text-orange-500";
      break;
    case 4:
      airQuality = "Unhealty";
      airQualityIndexColor = "text-red-500";
      break;
    case 5:
      airQuality = "Very Unhealty";
      airQualityIndexColor = "text-red-900";
      break;
  }

  const result = `<span class="text-xl lg:text-2xl xl:text-3xl font-semibold ${airQualityIndexColor}">${airQuality}</span>`;

  return result;
}


function getWeather() {
  const API_KEY = "60e76c0988a689b9a9e97d1570352775";
  const CURRENT_WEATHER_API = `https://api.openweathermap.org/data/2.5/weather?q=${currentLocation}&appid=${API_KEY}&units=${units}`;

  fetch(CURRENT_WEATHER_API)
  .then((response) => response.json())
  .then((data) => {
    weatherIcon.innerHTML = `<img src="https://openweathermap.org/img/wn/${data.weather[0].icon}@2x.png">`;
    weatherStatus.innerHTML = `<p>${data.weather[0].description}`;
    datetime.innerHTML = getFormatDate(data.dt);
    temperature.innerHTML = `${data.main.temp.toFixed()}`;
    displayLocation.innerHTML = `${data.name}, ${data.sys.country} | ${getFormatTime(data.dt, data.timezone)}`;
    minMax.innerHTML = `
        <span class="mr-4">H: ${data.main.temp_max.toFixed()}&#176</span>
        <span>L: ${data.main.temp_min.toFixed()}&#176</span>
    `;
    feel.innerHTML = `${data.main.feels_like.toFixed()}&#176`;
    humidity.innerHTML = `${data.main.humidity}&#37`;
    windSpeed.innerHTML = `${data.wind.speed} ${units === "imperial" ? "mph" : "m/s"}`;
    pressure.innerHTML = `${data.main.pressure} hPa`;
    clouds.innerHTML = `${data.clouds.all}&#37`;
    sunrise.innerHTML = getFormatTime(data.sys.sunrise, data.timezone);
    sunset.innerHTML = getFormatTime(data.sys.sunset, data.timezone);

    const AIR_QUALITY_API = `https://api.openweathermap.org/data/2.5/air_pollution?lat=${data.coord.lat}&lon=${data.coord.lon}&appid=${API_KEY}`;

    fetch(AIR_QUALITY_API)
    .then((response) => response.json())
    .then((data) => {
      airQuality.innerHTML = getAirQuality(data.list[0].main.aqi);
    });
  });

  const FIVE_DAY_FORECAST_API = `https://api.openweathermap.org/data/2.5/forecast?q=${currentLocation}&appid=${API_KEY}`

  fetch(FIVE_DAY_FORECAST_API)
  .then((response) => response.json())
  .then((data) => {
    // next 48 hours forecast
    for (let i = 0; i < next48Times.length; i++) {
      next48Times[i].innerHTML = `${getFormatHour(data.list[i].dt, data.city.timezone)}`;
      next48Icons[i].innerHTML = `<img class="w-14 h-14 object-cover" src="https://openweathermap.org/img/wn/${data.list[i].weather[0].icon}@2x.png" />`;
      nect48Temps[i].innerHTML = `${convertKelvin(units, data.list[i].main.temp)}&#176`;
    }

    // filter the forecast to get only one forecast per day
    const unixForecastDays = [];
    const dataForecast = data.list.filter(forecast => {
      const forecastDate = new Date(forecast.dt_txt).getDate();
      if (!unixForecastDays.includes(forecastDate)) {
        return unixForecastDays.push(forecastDate);
      }
    });

    // next 5 days forecast
    for (let i = 0; i < dateDay.length; i++) {
      if (i != 0) {
        dateDay[i].innerHTML = `${getFormatDate(dataForecast[i].dt)}`;
        iconDay[i].innerHTML = `<img class="w-14 h-14" src="https://openweathermap.org/img/wn/${dataForecast[i].weather[0].icon}@2x.png" />`;
        tempDay[i].innerHTML = `${convertKelvin(units, dataForecast[i].main.temp)}&#176`;
        statDay[i].innerHTML = `${dataForecast[i].weather[0].description}`;
      }
    }
  });
}

document.body.addEventListener("load", getWeather());
