/*
 *  The Mana World Server
 *  Copyright 2006 The Mana World Development Team
 *
 *  This file is part of The Mana World.
 *
 *  The Mana World is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  The Mana World is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with The Mana World; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  $Id$
 */

#ifndef _TMW_SERVER_MAPCOMPOSITE_
#define _TMW_SERVER_MAPCOMPOSITE_

#include <vector>

class Map;
class MapComposite;
class MovingObject;
class Object;
class Player;
class Point;
class Rectangle;
class Thing;

/**
 * Ordered sets of zones of a map.
 */
typedef std::vector< unsigned > MapRegion;

/**
 * Part of the map.
 */
struct MapZone
{
    unsigned short nbPlayers, nbMovingObjects;
    /**
     * Objects present in this zone.
     * Players are stored first, then the remaining MovingObjects, then the
     * remaining Objects.
     */
    std::vector< Object * > objects;

    /**
     * Destinations of the objects that left this zone.
     * This is necessary in order to have an accurate iterator around moving
     * objects.
     */
    MapRegion destinations;

    MapZone(): nbPlayers(0), nbMovingObjects(0) {}
    void insert(Object *);
    void remove(Object *);
};

/**
 * Iterates through the zones of a region of the map.
 */
struct ZoneIterator
{
    MapRegion region; /**< Zones to visit. Empty means the entire map. */
    unsigned pos;
    MapZone *current;
    MapComposite const *map;

    ZoneIterator(MapRegion const &, MapComposite const *);
    void operator++();
    MapZone *operator*() const { return current; }
    operator bool() const { return current; }
};

/**
 * Iterates through the Players of a region.
 */
struct PlayerIterator
{
    ZoneIterator iterator;
    unsigned short pos;
    Player *current;

    PlayerIterator(ZoneIterator const &);
    void operator++();
    Player *operator*() const { return current; }
    operator bool() const { return iterator; }
};

/**
 * Iterates through the MovingObjects of a region.
 */
struct MovingObjectIterator
{
    ZoneIterator iterator;
    unsigned short pos;
    MovingObject *current;

    MovingObjectIterator(ZoneIterator const &);
    void operator++();
    MovingObject *operator*() const { return current; }
    operator bool() const { return iterator; }
};

/**
 * Iterates through the non-moving Objects of a region.
 */
struct FixedObjectIterator
{
    ZoneIterator iterator;
    unsigned short pos;
    Object *current;

    FixedObjectIterator(ZoneIterator const &);
    void operator++();
    Object *operator*() const { return current; }
    operator bool() const { return iterator; }
};

/**
 * Iterates through the Objects of a region.
 */
struct ObjectIterator
{
    ZoneIterator iterator;
    unsigned short pos;
    Object *current;

    ObjectIterator(ZoneIterator const &);
    void operator++();
    Object *operator*() const { return current; }
    operator bool() const { return iterator; }
};

/**
 * Pool of public IDs for MovingObjects on a map. By maintaining public ID
 * availability using bits, it can locate an available public ID fast while
 * using minimal memory access.
 */
struct ObjectBucket
{
    static int const int_bitsize = sizeof(unsigned) * 8;
    unsigned bitmap[256 / int_bitsize]; /**< Bitmap of free locations. */
    short free;                         /**< Number of empty places. */
    short next_object;                  /**< Next object to look at. */
    MovingObject *objects[256];

    ObjectBucket();
    int allocate();
    void deallocate(int);
};

/**
 * Combined map/entity structure.
 */
class MapComposite
{
    public:
        /**
         * Constructor.
         */
        MapComposite(Map *);

        /**
         * Destructor.
         */
        ~MapComposite();

        Map *getMap() const
        { return map; }

        /**
         * Inserts an object on the map. Sets its public ID if relevant.
         */
        bool insert(Thing *);

        /**
         * Removes an object from the map.
         */
        void remove(Thing *);

        /**
         * Updates zones of every moving beings.
         */
        void update();

        /**
         * Gets an object given its ID.
         */
        MovingObject *getObjectByID(int) const;

        /**
         * Gets an iterator on the objects of the whole map.
         */
        ZoneIterator getWholeMapIterator() const
        { return ZoneIterator(MapRegion(), this); }

        /**
         * Gets an iterator on the objects inside a given rectangle.
         */
        ZoneIterator getInsideRectangleIterator(Rectangle const &) const;

        /**
         * Gets an iterator on the objects around a given point.
         */
        ZoneIterator getAroundPointIterator(Point const &, int radius) const;

        /**
         * Gets an iterator on the objects around a given object.
         */
        ZoneIterator getAroundObjectIterator(Object *, int radius) const;

        /**
         * Gets an iterator on the objects around the old and new positions of
         * a player (including the ones that were but are now elsewhere).
         */
        ZoneIterator getAroundPlayerIterator(MovingObject *, int radius) const;

        /**
         * Gets everything related to the map.
         */
        std::vector< Thing * > const &getEverything() const
        { return things; }

    private:
        MapComposite(MapComposite const &);

        /**
         * Allocates a unique ID for a moving object on this map.
         */
        bool allocate(MovingObject *);

        /**
         * Deallocates an ID.
         */
        void deallocate(MovingObject *);

        /**
         * Fills a region of zones within the range of a point.
         */
        void fillRegion(MapRegion &, Point const &, int) const;

        /**
         * Fills a region of zones inside a rectangle.
         */
        void fillRegion(MapRegion &, Rectangle const &) const;

        Map *map; /**< Actual map. */

        /**
         * Things (items, players, monsters, etc) located on the map.
         */
        std::vector< Thing * > things;

        /**
         * Buckets of MovingObjects located on the map, referenced by ID.
         */
        ObjectBucket *buckets[256];

        int last_bucket; /**< Last bucket acted upon. */

        /**
         * Partition of the Objects, depending on their position on the map.
         */
        MapZone *zones;

        unsigned short mapWidth;  /**< Width with respect to zones. */
        unsigned short mapHeight; /**< Height with respect to zones. */

        friend class ZoneIterator;
};

#endif
