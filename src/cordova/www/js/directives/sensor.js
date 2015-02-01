'use strict';

angular.module('app').directive('sensor', function() {
    return {
        restrict: 'E',
        replace: true,
        scope: {
            data: '='
        },
        templateUrl: 'views/templates/sensor.html'
    }
});
