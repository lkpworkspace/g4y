import math
import time
from g4y import *

class RotateScript(GCom):
    def __init__(self):
        GCom.__init__(self)
        self.rotate_y = 0.0

    def update(self):
        speed = 90.0
        t = deltaTime()
        self.rotate_y = self.rotate_y + speed * t
        self.transform().setRotation(0.0, self.rotate_y, 0.0)


def build_scene(s):
    camera = GObj()
    grid = GObj()
    demo_obj = GObj()

    camera_com = GCamera()
    grid_com = GGrid()
    roam_com = RoamScript()
    demo_com = RotateScript()
    
    camera.setTag("GCamera")
    camera.addCom(camera_com)
    camera.addCom(roam_com)

    grid.addCom(grid_com)

    demo_obj.addCom(demo_com)

    s.addChild(camera)
    s.addChild(grid)
    s.addChild(demo_obj)


if __name__ == '__main__':
    w = GWorld()
    s = GScene()

    w.setScene(s)

    build_scene(s)

    w.run()
