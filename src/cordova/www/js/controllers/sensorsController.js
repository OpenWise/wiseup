'use strict';

angular.module('app').controller('sensorsController', function($scope, sensorsFactory, sensorsService, serverService) {

    $scope.data = sensorsService;

    var eventSourceCallback = function(sid) {
        return function(event) {
            $scope.data.sensors[sid].value = event.data;
            console.log("sid " + sid + " received sse " + event.data);
            $scope.$apply();
        }
    }

    sensorsFactory.getAll(function() {
        for (var id in $scope.data.sensors) {
            console.log("sensors: " + id);
            var source = new EventSource(serverService.server + "sse/sensor/" + id);
            source.onmessage = eventSourceCallback(id);
        }
    });
});
