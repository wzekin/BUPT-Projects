using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

public class HouseController : MonoBehaviour
{
    //鼠标指针图片
    public Texture2D cursora;
    public Texture2D cursorb;

    //订单编号
    public static int index = 0;

    //鼠标是否按下的flag
    public static bool isMouseDown = false;

    //待添加的餐馆坐标
    public static Point fromPoint;

    //待添加的食客坐标
    public static Point toPoint;

    //model模型类,用于添加订单
    private ModelController model;

    void Start()
    {
        //初始化model
        model = GameObject.FindGameObjectWithTag("Model").GetComponent<ModelController>();
    }

    //鼠标按下时，标记餐馆坐标，并更改鼠标样式
    void OnMouseDown()
    {
        fromPoint = new Point(8 - System.Convert.ToInt32(transform.position.z - 0.5),
            System.Convert.ToInt32(transform.position.x + 7.5));
        isMouseDown = true;
        Cursor.SetCursor(cursorb, Vector2.zero, CursorMode.Auto);
    }
    
    void OnMouseEnter()
    {
        //鼠标按下拖曳至房屋时，更新食客坐标
        if (isMouseDown)
        {
            toPoint = new Point(8 - System.Convert.ToInt32(transform.position.z - 0.5),
                System.Convert.ToInt32(transform.position.x + 7.5));
        }
        //更新鼠标样式
        else
        {
            Cursor.SetCursor(cursora, Vector2.zero, CursorMode.Auto);
        }
    }
    void OnMouseExit()
    {
        //鼠标按下离开房屋时，更新食客坐标
        if (isMouseDown)
        {
            toPoint = null;
        }
        //更新鼠标样式
        else
        {
            Cursor.SetCursor(null, Vector2.zero, CursorMode.Auto);
        }
    }
    //鼠标松开时，如果在房屋上，则添加订单
    void OnMouseUp()
    {
        isMouseDown = false;
        if (toPoint != null)
        {
            model.AddOrder(
                index++,
                fromPoint.x,
                fromPoint.y,
                toPoint.x,
                toPoint.y
                );
            toPoint = null;
            Cursor.SetCursor(cursora, Vector2.zero, CursorMode.Auto);
        }
        else
        {
            Cursor.SetCursor(null, Vector2.zero, CursorMode.Auto);
        }
    }
}
