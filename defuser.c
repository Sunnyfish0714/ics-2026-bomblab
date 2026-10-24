#include "defuser.h"

#include <inttypes.h>
#include <stdio.h>

#define MAX_FIELDS 8

/*
 * 把 values 中的 count 个整数写成一行“空格分隔的无符号十进制整数”，存入 out。
 * 成功返回 0；count 为 0（还没有实现）、snprintf 出错或结果被截断时返回非零。
 */
static int write_answer(char *out, size_t cap, const uint32_t *values, size_t count) {
    size_t used = 0;
    for (size_t i = 0; i < count; ++i) {
        int size = snprintf(out + used, cap - used, "%s%" PRIu32, i ? " " : "", values[i]);
        if (size < 0 || (size_t)size >= cap - used) return 1;
        used += (size_t)size;
    }
    return count == 0;
}

int solve_phase_1(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    // TODO: 根据 phase_01 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    uint32_t low = (uint32_t)challenge;
    uint32_t high = (uint32_t)(challenge >> 32);
    uint32_t mid = (uint32_t)(challenge >> 16);

    uint32_t x = (low + 2u * high) ^ 0xe2f0u;
    uint32_t y = (mid ^ 0xd243u) + 2u * high;
    uint32_t z = 3u * (y - low) + 0x4106u;

    values[0] = z ^ x;
    count = 1;
    return write_answer(out, cap, values, count);
}

int solve_phase_2(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    // TODO: 根据 phase_02 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    values[0] = (uint32_t)((challenge >> 16) & 0xfu) ^ 0x10u;
    for (uint32_t i = 1; i < 6u; ++i) {
        uint32_t shift = 20u + 4u * i;
        uint32_t part = (uint32_t)((challenge >> shift) & 0x3fu);
        values[i] = (part ^ (2u * values[i - 1])) + 6u * i;
    }
    count = 6;
    return write_answer(out, cap, values, count);
}

int solve_phase_3(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    // TODO: 根据 phase_03 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    uint32_t index = (uint32_t)(challenge & 7u);
    uint32_t x = (uint32_t)(challenge >> 3)^ (uint32_t)(challenge >> 32);
    values[0] = index;
    switch (index) {
        case 0:
            values[1] = (x ^ 0x3f27u) + (x >> 1);
            break;
        case 1:
            values[1] = 5u * x + 0x48143u;
            break;
        case 2:
            values[1] = 3u * x + 0x7e69u;
            break;
        case 3:
            values[1] = (x ^ 0xcd68u) + (x >> 1);
            break;
        case 4:
            values[1] = (2u * x) ^ 0x1f73u;
            break;
        case 5:
            values[1] = 5u * x + 0xc173u;
            break;
        case 6:
            values[1] = 5u * x + 0x1cebcu;
            break;
        case 7:
            values[1] = (2u * x) ^ 0x508au;
            break;
    }
    count = 2;
    return write_answer(out, cap, values, count);
}

int solve_phase_4(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    // TODO: 根据 phase_04 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    uint32_t target = (uint32_t)(challenge & 0x7fu);
    uint32_t key = (uint32_t)(challenge >> 7) ^ (uint32_t)(challenge >> 32);
    uint32_t low = 0;
    uint32_t high = 127;
    uint32_t path_mix[8];
    size_t depth = 0;
    while (1) {
        uint32_t mid = low + ((high - low) >> 1);
        if (target == mid) {
            values[0] = (mid + 0x62u) ^ key;
            break;
        }
        if (target < mid) {
            path_mix[depth++] = (mid + key) ^ 1u;
            high = mid - 1u;
        } else {
            path_mix[depth++] = (mid + key) ^ 0xdu;
            low = mid + 1u;
        }
    }
    while (depth > 0) {
        --depth;
        values[0] = (5u * values[0]) ^ path_mix[depth];
    }
    count = 1;
    return write_answer(out, cap, values, count);
}

int solve_phase_5(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    // TODO: 根据 phase_05 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    struct NodeData {
        uint32_t next;
        uint32_t id;
        uint32_t value;
    };

    const struct NodeData nodes[12] = {
        {10u,        0x0b6eu, 0xb058u},
        {6u,         0x0a85u, 0x711au},
        {3u,         0x050cu, 0x989cu},
        {8u,         0x097cu, 0x3011u},
        {1u,         0x073cu, 0xc8f4u},
        {4u,         0x0350u, 0x1283u},
        {11u,        0x0455u, 0x12c1u},
        {0u,         0x0114u, 0xe14fu},
        {UINT32_MAX, 0x026fu, 0x0b46u},
        {7u,         0x0011u, 0x6b4fu},
        {2u,         0x0882u, 0x2191u},
        {9u,         0x0675u, 0xe33eu}
    };

    const uint32_t heads[8] = {
        5u, 4u, 1u, 6u, 11u, 9u, 7u, 0u
    };

    uint32_t head_index = (uint32_t)(challenge & 7u);
    uint32_t steps = (uint32_t)((challenge >> 3) & 3u);

    uint32_t state = (uint32_t)(challenge >> 6) ^ (uint32_t)(challenge >> 32) ^ 0xb8a6u;

    uint32_t current = heads[head_index];

    for (uint32_t i = 0; i <= steps; ++i) {
        state = (3u * state + nodes[current].value) ^ nodes[current].id;

        values[1] = nodes[current].id;

        if (i < steps) {
            current = nodes[current].next;
        }
    }

    values[0] = state;
    count = 2;
    return write_answer(out, cap, values, count);
}

int solve_phase_6(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    // TODO: 根据 phase_06 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    const uint32_t op_map[8] = {
        5u, 5u, 1u, 3u, 4u, 4u, 0u, 2u
    };

    uint32_t selectors[2] = {
        (uint32_t)(challenge & 7u),
        (uint32_t)((challenge >> 3) & 7u)
    };

    uint32_t state = (uint32_t)(challenge >> 6) ^ (uint32_t)(challenge >> 32);

    for (uint32_t stage = 0; stage < 2u; ++stage) {
        switch (op_map[selectors[stage]]) {
            case 0:
                state = (4u * state + 0x1224u) ^ (5u * state);
                break;

            case 1:
                state = ((state >> 5) ^ state) + 0x17fau;
                break;

            case 2:
                state = ((state << 7) | (state >> 25)) ^ 0xfb0bu;
                break;

            case 3:
                state = (state >> 7) | (state << 25);
                break;

            case 4:
                state = (state ^ 0x6425u) + (state << 5);
                break;

            case 5:
                state = (state + 0x8b6eu) ^ (state >> 3);
                break;
        }

        if (stage == 0u) {
            state ^= 0x36e5u;
        }
    }

    values[0] = state + 0xb354u;
    count = 1;
    return write_answer(out, cap, values, count);
}

int solve_secret(uint64_t challenge, uint64_t context, char *out, size_t cap) {
    (void)challenge;
    (void)context;
    (void)out;
    (void)cap;
    // TODO（选做）: 根据 secret_gate、secret_copy 和 secret_phase 的汇编构造完整的第七行，写入 out 后返回 0。
    // 第七行不一定只有整数，可以直接用 snprintf 写入；还没完成时保持返回 1，测试会显示 not attempted。
    uint32_t a = (uint32_t)challenge ^ (uint32_t)(challenge >> 32);

    uint32_t b = (uint32_t)context ^ (uint32_t)(context >> 32);

    uint32_t v0 = (uint32_t)(context & 3u);

    uint32_t mixed = a ^ b;
    uint32_t rotated = (mixed >> 7) | (mixed << 25);

    uint32_t v1 = rotated ^ 0xf13du ^ (v0 * 0x3348u) ^ (uint32_t)(context >> 16);

    uint32_t v2 = v1 ^ (b + v0 + 0xc275u + (a >> 7));

    const char padding[] =
        "AAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAA";

    int size = snprintf(
        out, cap,
        "%sOPEN%" PRIu32 " %" PRIu32 " %" PRIu32,
        padding, v0, v1, v2
    );

    if (size >= 0 && (size_t)size < cap) {
        return 0;
    }
    return 1;
}
