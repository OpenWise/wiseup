'use strict';

var TEMPERATURE_TYPE    = 1;
var LUMINANCE_TYPE      = 2;
var MOVEMENT_TYPE       = 3;
var SWITCH_TYPE         = 4;

angular.module('app').directive('sensor', function($http) {
    return {
        restrict: 'E',
        replace: true,
        scope: {
            data: '='
        },
        templateUrl: 'views/templates/sensor.html',
        link: function(scope, element, attrs) {
            scope.onClickSensor = function(type) {
                switch (type) {
                    case TEMPERATURE_TYPE:
                        alert ("TEMPERATURE_TYPE");
                    break;
                    case LUMINANCE_TYPE:
                        alert ("LUMINANCE_TYPE");
                    break;
                    case MOVEMENT_TYPE:
                        alert ("MOVEMENT_TYPE");
                    break;
                    case SWITCH_TYPE:
                        SendCommand ($http, element);
                    break;
                }
            }
        }
    }
    
    function SendCommand ($http, element) {
        var currentName = angular.element(element[0].querySelector('.sensor-name')).html();
        var currentValue = parseInt(angular.element(element[0].querySelector('.sensor-value')).html());

        $http.get('http://10.0.0.15/api/sensors/' + currentName + '/' + (currentValue+1)%2).
            success(function(data, status, headers, config) {
            }).
            error(function(data, status, headers, config) {
        });
    }
});
