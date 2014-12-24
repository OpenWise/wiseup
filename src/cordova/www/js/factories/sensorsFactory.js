'use strict';

angular.module('app').factory('sensorsFactory', function($resource, sensorsService, serverService) {
    return $resource(serverService.server + 'api/sensors', null, {
        getAll: {
            method: "GET",
            interceptor: {
                response: function(response) {
                    sensorsService.sensors = response.data;
                    return response.data;
                }
            }
        }
    });
});
