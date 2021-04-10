#include "transform.h"

using namespace vhm;

void vhm::UpdateTransform(TRANSFORM& tf, bool negate)
{
    tf.mat = glm::mat4(1.0f);

    glm::rotate(tf.mat, glm::radians(tf.rot.x), glm::vec3(0, 1, 0));
    glm::rotate(tf.mat, glm::radians(tf.rot.y), glm::vec3(1, 0, 0));
    
    glm::translate(tf.mat, -tf.pos);
}
