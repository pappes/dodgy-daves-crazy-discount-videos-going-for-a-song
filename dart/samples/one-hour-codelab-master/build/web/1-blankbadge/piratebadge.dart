// Copyright (c) 2012, the Dart project authors.  Please see the AUTHORS file
// for details. All rights reserved. Use of this source code is governed by a
// BSe license that can be found in the LICENSE file.

import 'dart:html';
import 'dart:math' show Random;
import 'dart:convert' show JSON;
import 'dart:async' show Future;
ButtonElement genButton;
SpanElement badgeNameElement;
final String TREASURE_KEY = 'pirateName';


void main() {
  // Your app starts here.
  InputElement inputField = querySelector('#inputName');
  inputField.onInput.listen(updateBadge);
  genButton = querySelector('#generateButton');
  genButton.onClick.listen(generateBadge);
  badgeNameElement = querySelector('#badgeName');
  
  PirateName.readyThePirates()
    .then((_) {
      //on suvcess
    inputField.disabled = false; //enable
    genButton.disabled = false; //enable
    setBadgeName(getBadgeNameFromStorage());
  })
  .catchError((arrr) {
    print('Error initialising pirate names: $arrr');
    badgeNameElement.text = 'Arr No Names!';
  });

}
  void updateBadge(Event e) {
    String inputName = (e.target as InputElement).value;
    setBadgeName(new PirateName(firstName:inputName));
    if (inputName.trim().isEmpty){
      genButton..disabled = false
          ..text = 'Aye!   Gimmie a name!';
    } else {
      genButton..disabled = true
          ..text='Arrr!  Write yer name!';
    }
  }
  
  void setBadgeName(PirateName newName){
    if (newName == null) {
      return;
    }
    querySelector('#badgeName').text = newName.pirateName;
    window.localStorage[TREASURE_KEY] = newName.jsonString;
  }
  
  PirateName getBadgeNameFromStorage() {
    String storedName = window.localStorage[TREASURE_KEY];
    if (storedName != null) {
      return new PirateName.fromJSON(storedName);
    } else {
      return null;
    }
  }
  
  void generateBadge(Event e){
    setBadgeName(new PirateName());
  }
  
  class PirateName {
    static final Random indexGen = new Random();
    
    static List<String> names = [];
    static List<String> appellations = [ ]; 
  
  
    String _firstName;
    String _appellation;
        
  
    String get pirateName {return 
        _firstName.isEmpty ? '' : '$_firstName the $_appellation';}
    String get jsonString {return JSON.encode( 
        {'f':_firstName, 'a':_appellation});}
    
    PirateName({String firstName, String appellation}) {
      if (firstName == null) {
         _firstName = names[indexGen.nextInt(names.length)];
       } else {
         _firstName=firstName;
       }
      if (appellation == null) {
         _appellation = appellations[indexGen.nextInt(appellations.length)];
       } else {
         _appellation=appellation;
       }       
    }
  
    PirateName.fromJSON(String jsonString) {
      Map storedName = (JSON.decode(jsonString) as Map);
      _firstName = storedName['f'];
      _appellation = storedName['a'];
    }
  
    static Future readyThePirates(){
      String path = 'pirateBadge.json';
      return HttpRequest.getString(path)
          .then(_parsePirateNamesFromJSON);
    }
    
    static _parsePirateNamesFromJSON(String jsonString){
      Map pirateNames = JSON.decode(jsonString);
      names = pirateNames['names'];
      appellations = pirateNames['appellations'];
    }
  }