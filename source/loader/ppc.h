#ifndef RRC_PPC_H
#define RRC_PPC_H

#define _RRC_PPC_ADDIS_OP (0b001111 << 26)
#define _RRC_PPC_ORI_OP (0b011000 << 26)
#define _RRC_PPC_XFORM_OP (0b011111 << 26)
#define _RRC_PPC_MTSPR_XFORM (0b0111010011 << 1)
#define _RRC_PPC_BCCTR_OP (0b010011 << 26)
#define _RRC_PPC_BCCTR_XFORM (0b1000010000 << 1)

// Creates a `lis` instruction (shift `val` to the left by 16 bits and store it in `reg`).
#define RRC_PPC_LIS(reg, val) \
    (_RRC_PPC_ADDIS_OP | ((reg) << 21) | ((val) & 0xffff))

// Creates an `ori` instruction (bitwise OR `val` with the value in `src_reg` and store it in `dest_reg`).
#define RRC_PPC_ORI(dest_reg, src_reg, val) \
    (_RRC_PPC_ORI_OP | ((src_reg) << 21) | ((dest_reg) << 16) | ((val) & 0xffff))

// Creates a `mtctr` instruction (move gpr `reg` into the ctr register).
#define RRC_PPC_MTCTR(reg) \
    (_RRC_PPC_XFORM_OP | _RRC_PPC_MTSPR_XFORM | ((reg) << 21) | (9 << 16 /* CTR */))

// Creates a `bctr` instruction (unconditionally branch to the address in CTR).
#define RRC_PPC_BCTR \
    (_RRC_PPC_BCCTR_OP | _RRC_PPC_BCCTR_XFORM | (0b10100 << 21 /* Branch Always*/))

// Expands to an array initializer containing the 4 necessary instructions to branch to an arbitrary 32-bit address `addr`.
#define RRC_PPC_BRANCH(addr)              \
    {                                     \
        RRC_PPC_LIS(9, addr >> 16),       \
        RRC_PPC_ORI(9, 9, addr & 0xffff), \
        RRC_PPC_MTCTR(9),                 \
        RRC_PPC_BCTR,                     \
    }

#endif
