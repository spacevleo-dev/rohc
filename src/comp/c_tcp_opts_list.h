/*
 * Copyright 2012,2013,2014,2015 Didier Barvaux
 * Copyright 2013,2014 Viveris Technologies
 * Copyright 2012 WBX
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

/**
 * @file   c_tcp_opts_list.h
 * @brief  Handle the list of TCP options for the TCP ompression profile
 * @author FWX <rohc_team@dialine.fr>
 * @author Didier Barvaux <didier@barvaux.org>
 * @author Didier Barvaux <didier.barvaux@toulouse.viveris.com>
 */

#ifndef ROHC_COMP_TCP_OPTS_LIST_H
#define ROHC_COMP_TCP_OPTS_LIST_H

#include "rohc_comp_internals.h"
#include "protocols/tcp.h"
#include "protocols/rfc6846.h"

#include <stdint.h>
#include <stddef.h>


/**
 * @brief The compression context for one TCP option
 */
struct __attribute__((aligned(8))) c_tcp_opt_ctxt
{
	/** The TCP option data (without type and length fields) */
	uint8_t payload[ROHC_TCP_OPT_MAX_LEN - 2];
	/** The number of times the full TCP option was transmitted */
	uint8_t full_trans_nr;
	/** The number of times the dynamic part of TCP option was transmitted */
	uint8_t dyn_trans_nr;
	/** The type of the TCP option */
	uint8_t type;
	/** Whether the option context is in use or not */
	bool used;
	uint8_t age;
	/** The length of the TCP option */
	uint8_t data_len;
	uint8_t unused[2];
} __attribute__((packed));

/* compiler sanity check for C11-compliant compilers and GCC >= 4.6 */
#if ((defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L) || \
     (defined(__GNUC__) && defined(__GNUC_MINOR__) && \
      (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6))))
_Static_assert((sizeof(sack_block_t) * 4) == (ROHC_TCP_OPT_MAX_LEN - 2),
               "ROHC_TCP_OPT_MAX_LEN shall be large enough for 4 SACK blocks");
_Static_assert((offsetof(struct c_tcp_opt_ctxt, payload) % 8) == 0,
               "payload in c_tcp_opt_ctxt should be aligned on 8 bytes");
_Static_assert((sizeof(struct c_tcp_opt_ctxt) % 8) == 0,
               "c_tcp_opt_ctxt length should be multiple of 8 bytes");
#endif


/** The temporary part of the compression context for TCP options */
struct __attribute__((aligned(8))) c_tcp_opts_ctxt_tmp
{
	/** The value of the TCP option timestamp echo request (in HBO) */
	uint32_t ts_req;
	/** The value of the TCP option timestamp echo reply (in HBO) */
	uint32_t ts_reply;

	/** Does the TCP option changed? */
	struct
	{
		uint8_t used:1;            /**< Is index in use? */
		uint8_t is_index_recycled; /**< Is index recycled from another option type? */
		uint8_t static_changed:1;  /**< Does static part of option changed? */
		uint8_t dyn_changed:1;     /**< Does dynamic part of option changed? */
		uint8_t unused:5;
	}
	changes[MAX_TCP_OPTION_INDEX + 1];

	/** Is item of TCP option required in replicate or CO chain? */
	bool list_item_needed[MAX_TCP_OPTION_INDEX + 1];

	/** The mapping between option positions and indexes */
	uint8_t position2index[ROHC_TCP_OPTS_MAX];

	/* The maximum index value used for the list of TCP options */
	uint8_t idx_max:4;
	/** Whether the compressed list of TCP options shall be transmitted in
	 *  one of the IR, IR-DYN, IR-CR, co_common, seq_8, or rnd_8 packets */
	uint8_t is_list_needed:1;
	/** Whether the structure of the list of TCP options changed in the
	 * current packet */
	uint8_t do_list_struct_changed:1;
	uint8_t unused:2;

	uint8_t ts_req_bytes_nr:3;
	uint8_t ts_reply_bytes_nr:3;
	uint8_t unused2:2;

	uint8_t unused4[6];
};

/* compiler sanity check for C11-compliant compilers and GCC >= 4.6 */
#if ((defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L) || \
     (defined(__GNUC__) && defined(__GNUC_MINOR__) && \
      (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6))))
_Static_assert((offsetof(struct c_tcp_opts_ctxt_tmp, ts_req) % 8) == 0,
               "ts_req in c_tcp_opts_ctxt_tmp should be aligned on 8 bytes");
_Static_assert((offsetof(struct c_tcp_opts_ctxt_tmp, changes) % 8) == 0,
               "changes in c_tcp_opts_ctxt_tmp should be aligned on 8 bytes");
_Static_assert((offsetof(struct c_tcp_opts_ctxt_tmp, list_item_needed) % 8) == 0,
               "list_item_needed in c_tcp_opts_ctxt_tmp should be aligned on 8 bytes");
_Static_assert((offsetof(struct c_tcp_opts_ctxt_tmp, position2index) % 8) == 0,
               "position2index in c_tcp_opts_ctxt_tmp should be aligned on 8 bytes");
_Static_assert((sizeof(struct c_tcp_opts_ctxt_tmp) % 8) == 0,
               "c_tcp_opts_ctxt_tmp length should be multiple of 8 bytes");
#endif


/** The compression context for TCP options */
struct __attribute__((aligned(8))) c_tcp_opts_ctxt
{
	uint8_t old_structure[ROHC_TCP_OPTS_MAX];
	uint8_t old_structure_nr;
	struct c_tcp_opt_ctxt list[MAX_TCP_OPTION_INDEX + 1];

	struct c_wlsb ts_req_wlsb;
	struct c_wlsb ts_reply_wlsb;

	/** The number of times the structure of the list of TCP options was
	 * transmitted since it last changed */
	uint8_t structure_nr_trans;
	uint8_t unused[6];
};

/* compiler sanity check for C11-compliant compilers and GCC >= 4.6 */
#if ((defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L) || \
     (defined(__GNUC__) && defined(__GNUC_MINOR__) && \
      (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6))))
_Static_assert((offsetof(struct c_tcp_opts_ctxt, old_structure) % 8) == 0,
               "structure in c_tcp_opts_ctxt should be aligned on 8 bytes");
_Static_assert((offsetof(struct c_tcp_opts_ctxt, list) % 8) == 0,
               "list in c_tcp_opts_ctxt should be aligned on 8 bytes");
_Static_assert((offsetof(struct c_tcp_opts_ctxt, ts_req_wlsb) % 8) == 0,
               "ts_req_wlsb in c_tcp_opts_ctxt should be aligned on 8 bytes");
_Static_assert((offsetof(struct c_tcp_opts_ctxt, ts_reply_wlsb) % 8) == 0,
               "ts_reply_wlsb in c_tcp_opts_ctxt should be aligned on 8 bytes");
_Static_assert((sizeof(struct c_tcp_opts_ctxt) % 8) == 0,
               "c_tcp_opts_ctxt length should be multiple of 8 bytes");
#endif


bool rohc_comp_tcp_are_options_acceptable(const struct rohc_comp *const comp,
                                          const uint8_t *const opts,
                                          const size_t data_offset,
                                          struct rohc_pkt_hdrs *const uncomp_pkt_hdrs)
	__attribute__((warn_unused_result, nonnull(1, 2, 4)));

void tcp_detect_options_changes(const struct rohc_comp_ctxt *const context,
                                const struct rohc_pkt_hdrs *const uncomp_pkt_hdrs,
                                const struct c_tcp_opts_ctxt *const opts_ctxt,
                                struct c_tcp_opts_ctxt_tmp *const tmp,
                                const bool tcp_ack_num_changed)
	__attribute__((nonnull(1, 2, 3, 4)));

int c_tcp_code_tcp_opts_list_item(const struct rohc_comp_ctxt *const context,
                                  const struct rohc_pkt_hdrs *const uncomp_pkt_hdrs,
                                  const struct c_tcp_opts_ctxt_tmp *const tmp,
                                  const bool items_needed[MAX_TCP_OPTION_INDEX + 1],
                                  uint8_t *const comp_opts,
                                  const size_t comp_opts_max_len)
	__attribute__((warn_unused_result, nonnull(1, 2, 3, 5)));

int c_tcp_code_tcp_opts_irreg(const struct rohc_comp_ctxt *const context,
                              const struct rohc_pkt_hdrs *const uncomp_pkt_hdrs,
                              const struct c_tcp_opts_ctxt *const opts_ctxt,
                              const struct c_tcp_opts_ctxt_tmp *const tmp,
                              const bool items_not_needed[MAX_TCP_OPTION_INDEX + 1],
                              uint8_t *const comp_opts,
                              const size_t comp_opts_max_len)
	__attribute__((warn_unused_result, nonnull(1, 2, 3, 4, 6)));

#endif /* ROHC_COMP_TCP_OPTS_LIST_H */

