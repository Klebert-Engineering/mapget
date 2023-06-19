#pragma once

#include "cache.h"
#include "datasource.h"

namespace mapget
{

/**
 * Client request for map data, which consists of a map id,
 * a map layer id, an array of tile ids, and a callback function
 * which signals results.
 */
class Request
{
    friend class Service;

public:
    using Ptr = std::shared_ptr<Request>;

    /** Construct a request with the relevant parameters. */
    Request(
        std::string mapId,
        std::string layerId,
        std::vector<TileId> tiles,
        std::function<void(TileFeatureLayer::Ptr)> onResult);

    /** Check if the request has been fully satisfied. */
    [[nodiscard]] bool isDone() const;

    /** The map id for which this request is dedicated. */
    std::string mapId_;

    /** The map layer id for which this request is dedicated. */
    std::string layerId_;

    /**
     * The map tile ids for which this request is dedicated.
     * Must not be empty. Result tiles will be processed in the given order
     */
    std::vector<TileId> tiles_;

    /**
     * The callback function which is called when a
     */
    std::function<void(TileFeatureLayer::Ptr)> onResult_;

protected:
    // So the hub can track which tileId index from tiles_
    // is next in line to be processed.
    size_t nextTileIndex_ = 0;
};

/**
 * Class which serves to unify multiple data sources for multiple maps,
 * and a cache which may store/restore the output of any of these sources.
 * The hub maintains a number of worker threads for each source, depending
 * on the source's maxParallelJobs_.
 */
class Service
{
public:
    /**
     * Construct a hub with a shared Cache instance. Note: The Cache must not
     * be null. For a simple default cache implementation, you can use the
     * MemCache.
     */
    explicit Service(Cache::Ptr cache);

    /** Destructor. Stops all workers of the present data sources. */
    ~Service();

    /**
     * Add a data source. Worker threads will be launched as needed,
     * and incoming/present requests for the data source will start to be
     * processed. Note, that the map layer versions for all layers of the
     * given source must be compatible with present one's, if existing.
     */
    void add(DataSource::Ptr const& dataSource);

    /**
     * Remove a data source from the hub. Requests for data which
     * can only be satisfied by the given source will not be processed anymore.
     * TODO: Any such ongoing request will be forcefully marked as done.
     */
    void remove(DataSource::Ptr const& dataSource);

    /**
     * Request some map data tiles. Will throw an exception if
     * there is no worker which is able to process the request.
     * Note: The same request object should only ever be passed
     *  to one hub. Otherwise, there is undefined behavior.
     */
    void request(Request::Ptr r);

    /**
     * Abort the given request. The request will be removed from
     * the processing queue, and forcefully marked as done.
     */
    void abort(Request::Ptr const& r);

    /** DataSourceInfo for all data sources which have been added to this Hub. */
    std::vector<DataSourceInfo> info();

private:
    struct Impl;
    struct Worker;
    std::unique_ptr<Impl> impl_;
};

}  // namespace mapget
