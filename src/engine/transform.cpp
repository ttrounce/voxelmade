#include "transform.h"

using namespace vhm;

void vhm::UpdateTransform(TRANSFORM& tf, bool negate)
{
    glm_mat4_identity(tf.mat);
    vec2 rot = {glm_rad(tf.rot[0]), glm_rad(tf.rot[1])};
    vec3 yawAxis = {0, 1, 0};
    vec3 pitchAxis = {1, 0, 0};
    
    glm_rotate(tf.mat, rot[0], pitchAxis);
    glm_rotate(tf.mat, rot[1], yawAxis);

    vec3 translation = {tf.pos[0], tf.pos[1], tf.pos[2]};
    if(negate) {
        glm_vec3_negate(translation);
    }
    glm_translate(tf.mat, translation);
}
