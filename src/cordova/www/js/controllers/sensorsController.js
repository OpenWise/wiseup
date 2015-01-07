'use strict';

angular.module('app').controller('sensorsController', function($scope, sensorsFactory, sensorsService, serverService) {

    $scope.data = sensorsService;

    var eventSourceCallback = function(idx) {
        return function(event) {
            $scope.data.sensors[idx].value = event.data;
            console.log("sid " + $scope.data.sensors[idx]._id + " received sse " + event.data);
            $scope.$apply();
        }
    }

    sensorsFactory.getAll(function() {
        for (var i = 0; i < $scope.data.sensors.length; i++) {
            var s = $scope.data.sensors[i];
            console.log("sensors: " + s._id);
            var source = new EventSource(serverService.server + "sse/sensor/" + s._id);
            source.onmessage = eventSourceCallback(i);
        }
    });
});