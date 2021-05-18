/**
 * Copyright (C) Mellanox Technologies Ltd. 2021.  ALL RIGHTS RESERVED.
 *
 * See file LICENSE for terms.
 */

#ifndef UCC_TL_SHARP_H_
#define UCC_TL_SHARP_H_

#include "components/tl/ucc_tl.h"
#include "components/tl/ucc_tl_log.h"
#include "utils/ucc_mpool.h"
#include "utils/ucc_rcache.h"

#include <sharp/api/sharp.h>
#include <limits.h>

#ifndef UCC_TL_SHARP_DEFAULT_SCORE
#define UCC_TL_SHARP_DEFAULT_SCORE 30
#endif

typedef struct ucc_tl_sharp_iface {
    ucc_tl_iface_t super;
} ucc_tl_sharp_iface_t;

extern ucc_tl_sharp_iface_t ucc_tl_sharp;

typedef struct ucc_tl_sharp_lib_config {
    ucc_tl_lib_config_t super;
} ucc_tl_sharp_lib_config_t;

typedef struct ucc_tl_sharp_context_config {
    ucc_tl_context_config_t  super;
    struct sharp_coll_config cfg;
    char                    *dev_list;
    int                      use_rcache;
    size_t                   reg_threshold;
    unsigned int             rand_seed;
    unsigned int             uprogress_num_polls;
} ucc_tl_sharp_context_config_t;

typedef struct ucc_tl_sharp_lib {
    ucc_tl_lib_t super;
} ucc_tl_sharp_lib_t;
UCC_CLASS_DECLARE(ucc_tl_sharp_lib_t, const ucc_base_lib_params_t *,
                  const ucc_base_config_t *);

typedef struct ucc_tl_sharp_oob_ctx {
    void                   *ctx;
    ucc_context_oob_coll_t *ctx_oob;
    ucc_team_oob_coll_t    *team_oob;
} ucc_tl_sharp_oob_ctx_t;

typedef struct ucc_tl_sharp_reg {
    void *mr;
} ucc_tl_sharp_reg_t;

typedef struct ucc_tl_sharp_rcache_region {
    ucc_rcache_region_t super;
    ucc_tl_sharp_reg_t  reg;
} ucc_tl_sharp_rcache_region_t;

typedef struct ucc_tl_sharp_context {
    ucc_tl_context_t              super;
    struct sharp_coll_context    *sharp_context;
    ucc_tl_sharp_context_config_t cfg;
    ucc_mpool_t                   req_mp;
    ucc_tl_sharp_oob_ctx_t        oob_ctx;
    ucc_rcache_t                 *rcache;
} ucc_tl_sharp_context_t;
UCC_CLASS_DECLARE(ucc_tl_sharp_context_t, const ucc_base_context_params_t *,
                  const ucc_base_config_t *);

typedef struct ucc_tl_sharp_team {
    ucc_tl_team_t          super;
    void                  *oob_req;
    ucc_team_oob_coll_t    oob;
    struct sharp_coll_comm *sharp_comm;
    ucc_rank_t             rank;
    ucc_rank_t             size;
    ucc_tl_sharp_oob_ctx_t oob_ctx;
} ucc_tl_sharp_team_t;

typedef struct ucc_tl_sharp_task {
    ucc_coll_task_t             super;
    void                       *req_handle;
    union {
        struct {
            ucc_tl_sharp_reg_t *s_mem_h;
            ucc_tl_sharp_reg_t *r_mem_h;
        } allreduce;
    };
} ucc_tl_sharp_task_t;

#define TASK_TEAM(_task)                                                       \
    (ucc_derived_of((_task)->super.team, ucc_tl_sharp_team_t))
#define TASK_CTX(_task)                                                        \
    (ucc_derived_of((_task)->super.team->context, ucc_tl_sharp_context_t))
#define TASK_LIB(_task)                                                        \
    (ucc_derived_of((_task)->super.team->context->lib, ucc_tl_sharp_lib_t))
#define TASK_ARGS(_task) (_task)->super.bargs.args

#define UCC_TL_SHARP_SUPPORTED_COLLS                         \
      (UCC_COLL_TYPE_ALLREDUCE | UCC_COLL_TYPE_BARRIER)

UCC_CLASS_DECLARE(ucc_tl_sharp_team_t, ucc_base_context_t *,
                  const ucc_base_team_params_t *);

#define UCC_TL_SHARP_TEAM_LIB(_team)                                            \
    (ucc_derived_of((_team)->super.super.context->lib, ucc_tl_sharp_lib_t))

#endif
