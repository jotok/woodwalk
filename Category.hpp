#pragma once

namespace Category {
    enum Type {
        None = 0,
        SkyLayer = 1 << 0,
        TreeBackLayer = 1 << 1,
        TreeMiddleLayer = 1 << 2,
        TreeFrontLayer = 1 << 3,
        TreeLayer = TreeBackLayer | TreeMiddleLayer | TreeFrontLayer
    };
}

