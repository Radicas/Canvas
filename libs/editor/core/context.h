#ifndef CONTEXT_H_
#define CONTEXT_H_

#include <memory>

class QGraphicsScene;

class Context
{
   public:
    Context();

    const std::shared_ptr<QGraphicsScene>& getScene() const { return mScene; }

   private:
    std::shared_ptr<QGraphicsScene> mScene;
};

#endif /* CONTEXT_H_ */
