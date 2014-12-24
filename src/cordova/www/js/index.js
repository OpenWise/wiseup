'use strict';

var app = angular.module('app', ['ngRoute', 'ngResource']);

app.config(['$routeProvider',
    function($routeProvider) {
        $routeProvider
            .when('/', {
                templateUrl: "/views/sensors.html",
                controller: 'sensorsController'
            })
            .otherwise({
                redirectTo: '/'
            });
    }
]);
