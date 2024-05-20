#ifndef CONTEXT_H_
#define CONTEXT_H_

#include <memory>

class QGraphicsScene;

/**
 * @brief 在界面间传递关键结构用的上下文
 */
class Context
{
   public:
    Context();

    const std::shared_ptr<QGraphicsScene>& get_scene() const { return _scene; }

   private:
    std::shared_ptr<QGraphicsScene> _scene;
};

#endif /* CONTEXT_H_ */
