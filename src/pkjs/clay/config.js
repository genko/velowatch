var meta = require('../../../package.json');
module.exports = [
    {
        "type": "heading",
        "defaultValue": "VeloWatch"
    },
    {
        "type": "text",
        "defaultValue": "Contribute on <a href=\"https://github.com/genko/velowatch\">GitHub!</a>"
    },
    {
        "type": "section",
        "items": [
            {
                "type": "heading",
                "defaultValue": "Time",
            },
            {
                "type": "toggle",
                "label": "Leading zero",
                "messageKey": "timeLeadingZero",
            },
            {
                "type": "toggle",
                "label": "Show AM/PM",
                "messageKey": "timeShowAmPm",
            },
            {
                "type": "select",
                "label": "Axis time format",
                "messageKey": "axisTimeFormat",
                "defaultValue": "24h",
                "description": "Tip: go to Settings > Date & Time > Time Format on your watch to change the main time format",
                "options": [
                    {
                        "label": "12h",
                        "value": "12h"
                    },
                    {
                        "label": "24h",
                        "value": "24h"
                    }
                ]
            },
            {
                "type": "select",
                "label": "Main time font",
                "messageKey": "timeFont",
                "defaultValue": "roboto",
                "options": [
                    {
                        "label": "Roboto",
                        "value": "roboto"
                    },
                    {
                        "label": "Leco",
                        "value": "leco"
                    },
                    {
                        "label": "Bitham",
                        "value": "bitham"
                    },
                ]
            },
            {
                "type": "color",
                "label": "Main time color",
                "messageKey": "colorTime",
                "defaultValue": "#FFFFFF",
                "sunlight": false,
                "capabilities": ["COLOR"]
            },
        ]
    },
    {
        "type": "section",
        "items": [
            {
                "type": "heading",
                "defaultValue": "Calendar",
            },
            {
                "type": "color",
                "label": "Sunday color",
                "messageKey": "colorSunday",
                "defaultValue": "#FFFFFF",
                "sunlight": false,
                "capabilities": ["COLOR"]
            },
            {
                "type": "color",
                "label": "Saturday color",
                "messageKey": "colorSaturday",
                "defaultValue": "#FFFFFF",
                "sunlight": false,
                "capabilities": ["COLOR"]
            },
            {
                "type": "color",
                "label": "Public holidays color",
                "messageKey": "colorUSFederal",
                "defaultValue": "#FFFFFF",
                "description": "White (default) means disable",
                "sunlight": false,
                "capabilities": ["COLOR"]
            },
        ]
    },
    {
        "type": "section",
        "items": [
            {
                "type": "heading",
                "defaultValue": "RiDuck"
            },
            {
                "type": "input",
                "label": "Username",
                "messageKey": "riDuckUsername",
                "description": "Paste your RiDuck username here"
            },
            {
                "type": "input",
                "label": "Password",
                "messageKey": "riDuckPassword",
                "description": "Paste your RiDuck password here"
            }
        ]
    },
    {
        "type": "section",
        "items": [
            {
                "type": "heading",
                "defaultValue": "OpenHolidaysAPI"
            },
            {
                "type": "input",
                "label": "Country ISO code",
                "messageKey": "openHolidaysCountry",
                "description": "Enter the country ISO code to fetch public holidays"
            },
            {
                "type": "input",
                "label": "Country regional ISO code",
                "messageKey": "openHolidaysRegional",
                "description": "If needed enter the country regional ISO code to fetch public holidays"
            }
        ]
    },
    {
        "type": "section",
        "items": [
            {
                "type": "heading",
                "defaultValue": "Weather"
            },
            {
                "type": "select",
                "defaultValue": "f",
                "messageKey": "temperatureUnits",
                "label": "Temperature Units",
                "options": [
                    {
                        "label": "°F",
                        "value": "f"
                    },
                    {
                        "label": "°C",
                        "value": "c"
                    }
                ]
            },
            {
                "type": "radiogroup",
                "label": "Provider",
                "messageKey": "provider",
                "defaultValue": "openweathermap",
                "options": [
                    {
                        "label": "OpenWeatherMap",
                        "value": "openweathermap"
                    }
                ]
            },
            {
                "type": "input",
                "label": "OpenWeatherMap API key",
                "messageKey": "owmApiKey",
                "description": "<a href='https://openweathermap.org/'>Register an OpenWeatherMap account</a> and paste your API key here"
            },
            {
                "type": "toggle",
                "label": "Force weather fetch",
                "messageKey": "fetch",
                "description": "Last successful fetch:<br><span id='lastFetchSpan'>Never :(</span>"
            },
            {
                "type": "input",
                "label": "Location override",
                "messageKey": "location",
                "description": "Example: \"Manhattan\" or \"123 Oak St Plainsville KY\".<br><a href=\"https://locationiq.com/demo\">Click here</a> to test out your location query.<br>To use GPS, leave this blank and ensure GPS is enabled on your device.",
                "attributes": {
                    "placeholder": "Using GPS",
                }
            }
        ]
    },
    {
        "type": "section",
        "items": [
            {
                "type": "heading",
                "defaultValue": "Misc"
            },
            {
                "type": "toggle",
                "label": "Show quiet time icon",
                "messageKey": "showQt",
                "defaultValue": true
            },
            {
                "type": "toggle",
                "label": "Vibrate on bluetooth disconnect",
                "messageKey": "vibe",
                "defaultValue": false
            },
            {
                "type": "select",
                "defaultValue": "both",
                "messageKey": "btIcons",
                "label": "Show icon for bluetooth",
                "options": [
                    {
                        "label": "Disconnected",
                        "value": "disconnected"
                    },
                    {
                        "label": "Connected",
                        "value": "connected"
                    },
                    {
                        "label": "Both",
                        "value": "both"
                    },
                    {
                        "label": "None",
                        "value": "none"
                    }
                ]
            },
        ]
    },
    {
        "type": "submit",
        "defaultValue": "Save Settings"
    },
    {
        "type": "text",
        "defaultValue": "v" + meta.version
    }
]
