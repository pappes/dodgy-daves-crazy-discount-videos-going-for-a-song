// Copyright (c) 2015, pappes. All rights reserved. Use of this source code
// is governed by a BSD-style license that can be found in the LICENSE file.

/// Collection of utility classes and functions to make programs simpler.
library pappesUtility;

/// Arbitary storage of objects in memory for later use.
///
/// ## purpose 
/// 
/// * Objects are stored with a key for retrieval.
/// * The cache can be segmented per session 
///   to prevent unintended crosstalk for server side caches
/// * The cache is implemented with static methods so tht it can be uses 
///   without explicitily creating instances of [RamCache]
///   
/// ##Node Examples
///  
///     RamCache.remember('MyKey', value);
///     print(RamCache.recall('MyKey'));
///     RamCache.forget('MyKey');
///     
/// ##Session Examples
///  
///     RamCache.remember('MyKey', value, SessionGUID);
///     print(RamCache.recall('MyKey', SessionGUID));
///     RamCache.forget('MyKey', SessionGUID);
///     
/// ##Incorrect Examples
///  
///     var myCache = New RamCache(); //this is no need to use new
///     myCache.remember('MyKey', value); //this will use the shared node cache
///     
class RamCache {
  /// Store all the individual session cahces in a Map for later retrieval 
  static final Map<Object,RamCache> _nodeStore = new Map();
  /// Create a standalone cache for keys to be store in scope of the node
  static final Map _globalCache = new Map();
  /// Reference to a session cache for this instance of the [RamCache] class  
  final Map _sessionCache;
  
  /// Returns the Map that caches values for a [session] or if no [session]
  /// is passed this returns the global Map.
  static Map _lookupCache([session]) {
    return session==null ? _globalCache : _lookupSessionCache(session);
  }
  
  /// Finds the Map attached to [session].
  static Map _lookupSessionCache(session) {
    if (!_nodeStore.containsKey(session)) {
      new RamCache(session);
    }
    return _nodeStore[session]._sessionCache;
  }
  
  /// Stores a [value] in the cache for later retrival using [key].
  ///
  /// * [key] - the identifier that you will use to reference the stored value
  /// * [value] - the [Object] to be stored in the cache
  /// * [session] - optional second key used to partition the cache
  static remember(key, value, [session]) => _lookupCache(session)[key]=value;
 
  /// Retrieves a previously stored value from the cache using [key].
  ///
  /// * [key] - the identifier that you used to reference the stored value
  /// * [session] - optional second key used to partition the cache
  static Object recall(key, [session]) {
    return _lookupCache(session)[key];
  }  
  /// Removes a value from the cache perminantly.
  ///
  /// * [key] - the identifier that you used to reference the stored value
  /// * [session] - optional second key used to partition the cache
  static Object forget(key, [session]) {
    return _lookupCache(session).remove(key);
  }
  ///Removes all values for a particular [session].
  static destroySessionCache(session) => _nodeStore.remove(session);
  
  ///Creates a new [session] cache or returns an exisiting [session] cache.
  ///
  ///Constructor as invoked by external instanciations.
  factory RamCache([session]) {
    if (!_nodeStore.containsKey(session)) {
      _nodeStore[session] = new RamCache._initialise(new Map());
    }
    return _nodeStore[session];
  }
  
  ///Adds a session cache into the NodeStore
  ///
  ///Constructor as invoked by static instances of this class.    
  RamCache._initialise(this._sessionCache) {
  }

}
