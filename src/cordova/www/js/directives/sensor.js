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
                        alert ("SWITCH_TYPE");
                    break;
                }
            }
            
            switch (scope.data.type) {
                case TEMPERATURE_TYPE:
                    angular.element(element[0].querySelector('.sensor-action')).html("");
                break;
                case LUMINANCE_TYPE:
                    angular.element(element[0].querySelector('.sensor-action')).html("");
                break;
                case MOVEMENT_TYPE:
                    angular.element(element[0].querySelector('.sensor-action')).html("");
                break;
                case SWITCH_TYPE:
                    angular.element(element[0].querySelector('.sensor-value')).html("");
                break;
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

angular.module('app').directive('toggleButton', function($http) {
	return {
		require: 'ngModel',
		scope: {
			sensorId: '@sensorId',
            sensorVal: '@sensorVal',
			activeText: '@activeText',
			inactiveText: '@inactiveText',
			sensorState: '=ngModel'
		},
		replace: true,
		transclude: true,
		template: '<div>' +
			'<button class="pure-button" ng-class="{\'pure-button-primary\': state.value}" ng-click="state.toggle()">{{activeText}}</button>' +
			'<button class="pure-button" ng-class="{\'pure-button-primary\': !state.value}" ng-click="state.toggle()">{{inactiveText}}</button>' +
			'</div>',
		link: function postLink(scope) {
			scope.sensorState = "BLACK";
			scope.state = {
				value: false,
				color: "BLACK",
				toggle: function() {
					this.value = !this.value;
                    this.color = (this.color == "BLACK") ? "RED" : "BLACK";
					scope.sensorState = this.color;
					SendCommand ($http, scope.sensorId, (this.value == true) ? 1 : 0);
				}
			};
		}
	}
	
	function SendCommand ($http, sensorId, action) {
        $http.get('http://10.0.0.15/api/sensors/' + sensorId + '/' + action).
            success(function(data, status, headers, config) {
            }).
            error(function(data, status, headers, config) {
        });
    }
});
