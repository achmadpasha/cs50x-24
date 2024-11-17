# Weather Forecast Web Application

## Demo Video

Watch the [**demo here**](https://youtu.be/Mn5C1E3GHKU).

## Overview

The Weather Forecast Web Application is a fully functional, responsive web tool that allows users to quickly view current weather conditions, air quality, and forecasts for up to 5 days. Built using HTML, TailwindCSS, JavaScript, and integrated with the OpenWeatherMap API, the application provides an intuitive and visually appealing interface for users to track weather updates across different locations. It incorporates features such as switching between temperature units (Celsius/Fahrenheit), a city search bar, and a dynamic display of real-time weather data.

This project is aimed at providing a clean, interactive user experience while utilizing modern web technologies to deliver up-to-date weather information with additional useful data such as air pollution levels, humidity, and wind conditions. The application is designed to be responsive, ensuring that users can access accurate weather details on various devices, from desktop screens to mobile phones.

## Technologies Used

- **HTML**: The backbone of the application, used for structuring the content and layout of the page.
- **TailwindCSS**: A utility-first CSS framework used to style the application and ensure a responsive, modern design.
- **JavaScript**: Handles the logic of the application, including API requests, DOM manipulation, and data display.
- **OpenWeatherMap API**: Provides the real-time weather data. This includes current weather data, air pollution information, and forecasts for the next 5 days and 48 hours.
- **AOS Library**: Adds smooth scrolling animations to the elements as users scroll through the page, making the interaction more dynamic and engaging.

## Features

### 1. City Search Input
The user can type in the name of any city to fetch the current weather details and forecast data. This feature is implemented with a simple input field that triggers an API call upon submission. The city name is then displayed along with the weather data, making it easy for users to find and track weather information for any location.

### 2. Temperature Unit Switch
Users can toggle between Celsius and Fahrenheit units for the displayed temperature. This provides flexibility for users from different regions, accommodating both metric and imperial systems.

### 3. Current Weather Status
The current weather section displays real-time weather details for the searched city, including:
- **Temperature**: Displays the current temperature in the selected unit (Celsius or Fahrenheit).
- **Humidity**: Shows the current humidity percentage.
- **Wind Speed**: Provides the wind speed in kilometers per hour or miles per hour.
- **Air Quality**: Shows the current air pollution level based on data from the OpenWeatherMap Air Pollution API.
- **Pressure**: Indicates the current atmospheric pressure.
- **Cloudiness**: Displays the percentage of cloud coverage in the sky.
- **Sunrise and Sunset Times**: Provides the time of sunrise and sunset for the location, making it easy for users to plan their day.
- **Date and Time**: Shows the current local date and time for the searched location.

### 4. Next 48 Hours
This feature provides a forecast for the next 48 hours at 3-hour intervals. Each forecast entry shows the time, weather condition, and expected temperature, giving users detailed short-term information on how the weather will evolve throughout the day.

### 5. Next 5 Days
The 5-day forecast section offers weather information for the next five days. Each day’s forecast includes the datetime, temperature, and weather condition (e.g., sunny, cloudy, rainy). This feature is especially useful for users planning their activities for the upcoming days.

## Files and Functionality

### `index.html`
The index.html file contains the basic structure and layout of the application. It defines the necessary sections for displaying the weather data and includes input fields for the city name and temperature unit toggle. The structure also integrates the AOS library for smooth animations and is linked to the CSS file for styling and the JavaScript file for functionality.

### `global.css` (TailwindCSS)
This file uses TailwindCSS to style the webpage. It contains utility classes to manage the layout, typography, spacing, colors, and responsiveness of the app. The layout is designed to adapt to different screen sizes, ensuring that the application looks great on both desktop and mobile devices. TailwindCSS makes it easy to customize and extend styles as needed.

### `app.js`
The app.js file is the core of the application, handling the logic behind fetching and displaying data. This file handles communication with the OpenWeatherMap API which includes functions to retrieve current weather, air pollution data, and forecast data by making HTTP requests to the relevant endpoints. The API returns data in JSON format, which is then parsed and used to dynamically update the page. It:
- Listens for user input and triggers the API requests.
- Uses the OpenWeatherMap API to retrieve current weather, air pollution data, and forecasts.
- Updates the DOM with the fetched data, dynamically displaying the weather details for the selected city.
- Implements the temperature unit toggle functionality (Celsius/Fahrenheit).
- Integrates the AOS library to animate page elements as they scroll into view.

## Conclusion

The Weather Forecast Web Application is a functional, user-friendly tool that provides real-time weather forecasts for any location worldwide. It combines powerful API integration with a responsive, visually appealing design powered by TailwindCSS. The use of JavaScript ensures that the application is interactive and dynamic, while the AOS library enhances the user experience by adding smooth scrolling animations.

This project demonstrates how to combine various web development technologies to create a practical and engaging web application. Whether you're checking the weather for the day or planning for the next few days, this app offers all the necessary information in a clean and modern interface.
