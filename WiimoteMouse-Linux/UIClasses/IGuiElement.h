#include <utility>
#include <vector>

class IGuiElement
{
    float mPosX;
    float mPosY;
    float mScale;
    IGuiElement* mpParent;
    std::vector<IGuiElement*> children;
public:
    virtual bool Create(float x, float y, float scale) = 0;
    virtual void Delete() = 0;

    virtual void Render() = 0;
    virtual void OnHover() = 0;
    virtual void OnPress() = 0;
    virtual void OnRelease() = 0;
    virtual void Update() = 0;

    void UpdateChildren();
    std::pair<float, float> GetPosition();
    float GetScale();
    void SetPosition(float x, float y);
    float SetScale(float scale);
};
