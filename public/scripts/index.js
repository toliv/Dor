var app = angular.module('StarterApp', ['ngMaterial', 'ngResource']); 

app.controller('AppCtrl', ['$scope', '$resource', function($scope, $resource){
  $scope.toggleSearch = false;
  var GUEST = $resource('/api/v1/guests', null, {
    'update': {method: 'PUT'}
  });

  $scope.content = [];

  $scope.headers = [
  {
    name: 'Name', 
    field: 'name'
  },{
    name:'Phone Number', 
    field: 'number'
  },{
    name: 'Password', 
    field: 'password'
  }
  ];
  
  GUEST.query(function(guests){
    $scope.content = $scope.content.concat(guests);
  });
  // $scope.digest();
  
  $scope.custom = {name: 'bold', number:'grey', password:'grey'};
  $scope.sortable = ['name', 'number'];
  $scope.thumbs = 'thumb';
  $scope.count = 10;

  $scope.guest = {};

  $scope.addGuest = function(){
    if($scope.guestForm.$valid){
      $scope.content.push($scope.guest);
      GUEST.save($scope.guest);
      $scope.guest = {};
      $scope.guestForm.$setPristine();
      $scope.guestForm.$setUntouched();
    }
  }

  $scope.addPermanentGuest = function(){
    $scope.guest.permanent = true;
    $scope.addGuest();
  }

  $scope.deleteGuest = function(guest){
    console.log('Deleting user ', guest);
    var idx = $scope.content.indexOf(guest);
    $scope.content.splice(idx,1);
    GUEST.delete({
      "id":guest.id
    });
  }

  $scope.updateGuest = function(guest){
    GUEST.update(guest);
  }
}]);
