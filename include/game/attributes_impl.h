static constexpr float g_speed_by_heaviness[6] = {
    0.125f,
    0.250f,
    0.375f,
    0.500f,
    0.625f,
    0.750f
};

enum attack_tier {
    ATTACK_BAD = 0,
    ATTACK_MID,
    ATTACK_GOOD,
    ATTACK_TIER_AMOUNT
};

static constexpr float g_attack_threshold_by_strength[6][ATTACK_TIER_AMOUNT] = { // |  BAD  |  MID  |  GOOD  |  CRITIC  |
    { [ATTACK_BAD] = 1.00f, [ATTACK_MID] = 0.00f , [ATTACK_GOOD] = 0.00f },      // | 100%  |   0%  |    0%  |      0%  |
    { [ATTACK_BAD] = 0.60f, [ATTACK_MID] = 0.95f , [ATTACK_GOOD] = 0.99f },      // |  60%  |  35%  |    4%  |      1%  |
    { [ATTACK_BAD] = 0.40f, [ATTACK_MID] = 0.90f , [ATTACK_GOOD] = 0.99f },      // |  40%  |  50%  |    9%  |      1%  |
    { [ATTACK_BAD] = 0.20f, [ATTACK_MID] = 0.70f , [ATTACK_GOOD] = 0.95f },      // |  20%  |  50%  |   25%  |      5%  |
    { [ATTACK_BAD] = 0.05f, [ATTACK_MID] = 0.45f , [ATTACK_GOOD] = 0.85f },      // |   5%  |  40%  |   40%  |     15%  |
    { [ATTACK_BAD] = 0.00f, [ATTACK_MID] = 0.20f , [ATTACK_GOOD] = 0.70f },      // |   0%  |  20%  |   50%  |     30%  |
};

