class ValidMotion:
    def __init__(self):
        self.coef = {
            "INIT_POSE_COEF": 5,
            "INIT_VIEW_COEF": 1,
            "WALK_STEP_COEF": 1,
            "WALK_SPEED_COEF": 15,
            "WALK_VIEW_COEF": 5,
            "TURN_DIR_COEF": 1,
            "TURN_POSE_COEF": 15,
            "TURN_VIEW_COEF": 3,
            "MOVE_DIR_COEF": 1,
            "MOVE_POSE_COEF": 11,
            "MOVE_VIEW_COEF": 2
        }

        self.motion = {}

        self.ref = {
            "init": {
                "key": 1,
                "list": [
                    "INIT_LOW_DOWN",
                    "INIT_LOW_OBLIQUE",
                    "INIT_LOW_UP",
                    "INIT_LOW_LEFT",
                    "INIT_LOW_RIGHT",
                    "INIT_MIDDLE_DOWN",
                    "INIT_MIDDLE_OBLIQUE",
                    "INIT_MIDDLE_UP",
                    "INIT_MIDDLE_LEFT",
                    "INIT_MIDDLE_RIGHT",
                    "INIT_HIGH_DOWN",
                    "INIT_HIGH_OBLIQUE",
                    "INIT_HIGH_UP",
                    "INIT_HIGH_LEFT",
                    "INIT_HIGH_RIGHT"
                ]
            },
            "walk_fast_down": {
                "key": 16,
                "list": [
                    "WALK_FAST_START_DOWN",
                    "WALK_FAST_END_DOWN",
                    "WALK_FAST_L_DOWN",
                    "WALK_FAST_R_DOWN"
                ]
            },
            "walk_fast_OBLIQUE": {
                "key": 21,
                "list": [
                    "WALK_FAST_START_OBLIQUE",
                    "WALK_FAST_END_OBLIQUE",
                    "WALK_FAST_L_OBLIQUE",
                    "WALK_FAST_R_OBLIQUE"
                ]
            },
            "walk_fast_UP": {
                "key": 26,
                "list": [
                    "WALK_FAST_START_UP",
                    "WALK_FAST_END_UP",
                    "WALK_FAST_L_UP",
                    "WALK_FAST_R_UP"
                ]
            },
            "walk_slow_down": {
                "key": 31,
                "list": [
                    "WALK_SLOW_START_DOWN",
                    "WALK_SLOW_END_DOWN",
                    "WALK_SLOW_L_DOWN",
                    "WALK_SLOW_R_DOWN"
                ]
            },
            "walk_slow_OBLIQUE": {
                "key": 36,
                "list": [
                    "WALK_SLOW_START_OBLIQUE",
                    "WALK_SLOW_END_OBLIQUE",
                    "WALK_SLOW_L_OBLIQUE",
                    "WALK_SLOW_R_OBLIQUE"
                ]
            },
            "walk_slow_up": {
                "key": 41,
                "list": [
                    "WALK_SLOW_START_UP",
                    "WALK_SLOW_END_UP",
                    "WALK_SLOW_L_UP",
                    "WALK_SLOW_R_UP"
                ]
            },
            "turn_left_low down": {
                "key": 47,
                "list": [
                    "TURN_LEFT_LOW_DOWN",
                    "TURN_RIGHT_LOW_DOWN"
                ]
            },
            "turn left low oblique": {
                "key": 50,
                "list": [
                    "TURN_LEFT_LOW_OBLIQUE",
                    "TURN_RIGHT_LOW_OBLIQUE"
                ]
            },
            "turn left low left": {
                "key": 56,
                "list": [
                    "TURN_LEFT_LOW_LEFT",
                    "TURN_RIGHT_LOW_LEFT"
                ]
            }, "turn left low right": {
                "key": 59,
                "list": [
                    "TURN_LEFT_LOW_RIGHT",
                    "TURN_RIGHT_LOW_RIGHT"
                ]
            }, "turn left high down": {
                "key": 62,
                "list": [
                    "TURN_LEFT_HIGH_DOWN",
                    "TURN_RIGHT_HIGH_DOWN"
                ]
            },
            "turn left high oblique": {
                "key": 65,
                "list": [
                    "TURN_LEFT_HIGH_OBLIQUE",
                    "TURN_RIGHT_HIGH_OBLIQUE"
                ]
            },
            "TURN_LEFT_HIGH_LEFT": {
                "key": 71,
                "list": [
                    "TURN_LEFT_HIGH_LEFT",
                    "TURN_RIGHT_HIGH_LEFT"
                ]
            },
            "TURN_LEFT_HIGH_RIGHT": {
                "key": 74,
                "list": [
                    "TURN_LEFT_HIGH_RIGHT",
                    "TURN_RIGHT_HIGH_RIGHT"
                ]
            },
            "MOVE_LEFT_LOW_DOWN": {
                "key": 78,
                "list": [
                    "MOVE_LEFT_LOW_DOWN",
                    "MOVE_RIGHT_LOW_DOWN",
                    "MOVE_LEFT_LOW_OBLIQUE",
                    "MOVE_RIGHT_LOW_OBLIQUE"
                ]
            },
            "MOVE_LEFT_LOW_LEFT": {
                "key": 84,
                "list": [
                    "MOVE_LEFT_LOW_LEFT",
                    "MOVE_RIGHT_LOW_LEFT",
                    "MOVE_LEFT_LOW_RIGHT",
                    "MOVE_RIGHT_LOW_RIGHT"
                ]
            },
            "MOVE_LEFT_MIDDLE_DOWN": {
                "key": 89,
                "list": [
                    "MOVE_LEFT_MIDDLE_DOWN",
                    "MOVE_RIGHT_MIDDLE_DOWN",
                    "MOVE_LEFT_MIDDLE_OBLIQUE",
                    "MOVE_RIGHT_MIDDLE_OBLIQUE"
                ]
            },
            "MOVE_LEFT_MIDDLE_LEFT": {
                "key": 95,
                "list": [
                    "MOVE_LEFT_MIDDLE_LEFT",
                    "MOVE_RIGHT_MIDDLE_LEFT",
                    "MOVE_LEFT_MIDDLE_RIGHT",
                    "MOVE_RIGHT_MIDDLE_RIGHT"

                ]
            },
            "MOVE_LEFT_HIGH_DOWN": {
                "key": 100,
                "list": [
                    "MOVE_LEFT_HIGH_DOWN",
                    "MOVE_RIGHT_HIGH_DOWN",
                    "MOVE_LEFT_HIGH_OBLIQUE",
                    "MOVE_RIGHT_HIGH_OBLIQUE"

                ]
            },
            "MOVE_LEFT_HIGH_LEFT": {
                "key": 106,
                "list": [
                    "MOVE_LEFT_HIGH_LEFT",
                    "MOVE_RIGHT_HIGH_LEFT",
                    "MOVE_LEFT_HIGH_RIGHT",
                    "MOVE_RIGHT_HIGH_RIGHT"
                ]
            },
            "MISSION_2_MINE_WALK_START": {
                "key": 167,
                "list": [
                    "MISSION_2_MINE_WALK_START",
                    "MISSION_2_MINE_WALK_L",
                    "MISSION_2_MINE_WLAK_R",
                    "MISSION_2_MINE_WALK_END"
                ]
            }
        }

        self.skills = {
            "MISSION_1_RED_DUMBLING": 162,
            "MISSION_3_HURDLING": 173,
            "MISSION_5_STAIR_UP": 177,
            "MISSION_5_STAIR_DOWN": 180,
            "MISSION_6_RIGHT_KICK": 185,
            "MISSION_7_YELLOW_DUMBLING": 188,
            "TAKE_REST": 193,
            "NIL": 0xff
        }

        for motion in self.ref:
            key = self.ref[motion]["key"]
            l = self.ref[motion]["list"]
            for index in range(l.__len__()):
                # print(l[index], "=>", key + index)
                self.motion[l[index]] = key + index
                pass
            # print()
            pass

        for name in self.skills:
            # print(name, "=>", self.skills[name])
            self.motion[name] = self.skills[name]
            pass

        self.macro = {
            "INIT_MOTION": lambda pose, view: self.motion["INIT_LOW_DOWN"] + self.coef["INIT_POSE_COEF"] * self.pose[
                pose] + self.coef["INIT_VIEW_COEF"] * self.view[view],
            "WALK_START_MOTION": lambda speed, view: self.motion["WALK_FAST_START_DOWN"] + self.coef[
                "WALK_SPEED_COEF"] * self.speed[speed] + self.coef["WALK_VIEW_COEF"] * self.view[view],
            "WALK_END_MOTION": lambda speed, view: self.motion["WALK_FAST_END_DOWN"] + self.coef["WALK_SPEED_COEF"] *
                                                   self.speed[speed] + self.coef["WALK_VIEW_COEF"] * self.view[view],
            "WALK_MOTION": lambda step, speed, view: self.motion["WALK_FAST_L_DOWN"] + self.coef["WALK_STEP_COEF"] *
                                                     self.step[step] + self.coef["WALK_SPEED_COEF"] * self.speed[
                                                         speed] + self.coef["WALK_VIEW_COEF"] * self.view[view],
            "TURN_MOTION": lambda direction, pose, view: self.motion["TURN_LEFT_LOW_DOWN"] + self.coef[
                "TURN_DIR_COEF"] * self.direction[direction] + self.coef["TURN_POSE_COEF"] * self.pose[pose] +
                                                         self.coef["TURN_VIEW_COEF"] * self.view[view],
            "MOVE_MOTION": lambda direction, pose, view: self.motion["MOVE_LEFT_LOW_DOWN"] + self.coef[
                "MOVE_DIR_COEF"] * self.direction[direction] + self.coef["MOVE_POSE_COEF"] * self.pose[pose] +
                                                         self.coef["MOVE_VIEW_COEF"] * self.view[view]
        }

        self.motion_init = {
            "LOW_DOWN": self.motion["INIT_LOW_DOWN"],
            "LOW_OBLIQUE": self.motion["INIT_LOW_OBLIQUE"],
            "LOW_UP": self.motion["INIT_LOW_UP"],
            "LOW_LEFT": self.motion["INIT_LOW_LEFT"],
            "LOW_RIGHT": self.motion["INIT_LOW_RIGHT"],

            "MIDDLE_DOWN": self.motion["INIT_MIDDLE_DOWN"],
            "MIDDLE_OBLIQUE": self.motion["INIT_MIDDLE_OBLIQUE"],
            "MIDDLE_UP": self.motion["INIT_MIDDLE_UP"],
            "MIDDLE_LEFT": self.motion["INIT_MIDDLE_LEFT"],
            "MIDDLE_RIGHT": self.motion["INIT_MIDDLE_RIGHT"],

            "HIGH_DOWN": self.motion["INIT_HIGH_DOWN"],
            "HIGH_OBLIQUE": self.motion["INIT_HIGH_OBLIQUE"],
            "HIGH_UP": self.motion["INIT_HIGH_UP"],
            "HIGH_LEFT": self.motion["INIT_HIGH_LEFT"],
            "HIGH_RIGHT": self.motion["INIT_HIGH_RIGHT"],
            "INIT_NIL": self.motion["NIL"]
        }

        self.pose = {
            "LOW": 0,
            "MIDDLE": 1,
            "HIGH": 2
        }

        self.view = {
            "DOWN": 0,
            "OBLIQUE": 1,
            "UP": 2,
            "LEFT": 3,
            "RIGHT": 4
        }

        self.speed = {
            "FAST": 0,
            "SLOW": 1
        }

        self.step = {
            "STEP_LEFT": 0,
            "STEP_RIGHT": 1
        }

        self.direction = {
            "DIR_LEFT": 0,
            "DIR_RIGHT": 1
        }

        self.foo_mod = {
            "CHECK": 0,
            "SET": 1
        }
        pass

    def RobotAction(self, n):
        for key, value in self.motion.items():
            if value == n:
                print(key, value)
                return
                pass
            pass
        print("Error!! Not Found Motion #", value)
        pass

    def foo(self, init_motion, mod):
        self.motion_init["prev"] = self.motion_init["INIT_NIL"]

        if mod == self.foo_mod["CHECK"]:
            if init_motion != self.motion_init["prev"]:
                self.RobotAction(init_motion)
                self.motion_init["prev"] = init_motion
                pass
            pass
        elif mod == self.foo_mod["SET"]:
            self.motion_init["prev"] = init_motion
            pass
        pass

        pass

    def action(self, init, motion):
        self.foo(init, self.foo_mod["CHECK"])
        self.RobotAction(motion)
        pass

    def ACTION_INIT(self, pose, view):
        self.RobotAction(self.macro["INIT_MOTION"](pose, view))
        pass

    def ACTION_WALK(self, speed, view, repeat):
        self.action(self.macro["INIT_MOTION"]("LOW", view), self.macro["WALK_START_MOTION"](speed, view))

        for i in range(repeat):
            self.RobotAction(self.macro["WALK_MOTION"]("STEP_LEFT", speed, view))
            self.RobotAction(self.macro["WALK_MOTION"]("STEP_RIGHT", speed, view))
            pass

        self.RobotAction(self.macro["WALK_END_MOTION"](speed, view))
        pass

    def ACTION_TURN(self, direction, pose, view, repeat):
        self.action(self.macro["INIT_MOTION"](pose, view), self.macro["TURN_MOTION"](direction, pose, view))

        for i in range(repeat - 1):
            self.RobotAction(self.macro["TURN_MOTION"](direction, pose, view))
            pass
        pass

    def ACTION_MOVE(self, direction, pose, view, repeat):
        self.action(self.macro["INIT_MOTION"](pose, view), self.macro["MOVE_MOTION"](direction, pose, view))

        for i in range(repeat - 1):
            self.RobotAction(self.macro["MOVE_MOTION"](direction, pose, view))
        pass

    def ACTION_MISSION(self, mission):
        pass

    def test(self, user_input, p=None, v=None, sp=None, d=None, r=None, m=None):
        command = {
            "ACTION_INIT": {
                "args": dict(pose=p, view=v),
                "func": self.ACTION_INIT
            },
            "ACTION_WALK": {
                "args": dict(speed=sp, view=v, repeat=r),
                "func": self.ACTION_WALK
            },
            "ACTION_TURN": {
                "args": dict(direction=d, pose=p, view=v, repeat=r),
                "func": self.ACTION_TURN
            },
            "ACTION_MOVE": {
                "args": dict(direction=d, pose=p, view=v, repeat=r),
                "func": self.ACTION_MOVE
            },
            "ACTION_MISSION": {
                "args": dict(mission=m),
                "func": self.ACTION_MISSION
            }
        }

        print("INPUT : ", user_input, command[user_input]["args"], "\t")
        print("Expected Value: ", self.generateMotion(user_input, p, v, sp, d, m))
        print("RESULT : ")
        command[user_input]["func"](**command[user_input]["args"])
        print("\n\n")
        pass

    def generateMotion(self, user_input, p=None, v=None, sp=None, d=None, m=None):
        command = {
            "ACTION_INIT": "INIT_{0}_{1}".format(p, v),
            "ACTION_WALK": "WALK_{0}_*_{1}".format(sp, v),
            "ACTION_TURN": "TURN_{0}_{1}_{2}".format(("LEFT" if d == "DIR_LEFT" else "RIGHT"), p, v),
            "ACTION_MOVE": "MOVE_{0}_{1}_{2}".format(("LEFT" if d == "DIR_LEFT" else "RIGHT"), p, v),
            "ACTION_MISSION": "MISSION_{0}".format(m),
        }

        return command[user_input]
        pass


if __name__ == '__main__':
    valid = ValidMotion()

    index = -1

    try:
        for pose in valid.pose:
            for view in valid.view:
                index += 1
                print(index)
                valid.test(user_input="ACTION_INIT", p=pose, v=view)
                pass
            pass

        for speed in valid.speed:
            for view in valid.view:
                if view in ["LEFT", "RIGHT"]: continue
                index += 1
                print(index)
                valid.test(user_input="ACTION_WALK", sp=speed, v=view, r=3)
                pass
            pass

        for pose in valid.pose:
            if pose in ["MIDDLE"]: continue
            for view in valid.view:
                if view in ["UP"]: continue
                for direction in valid.direction:
                    index += 1
                    print(index)
                    valid.test(user_input="ACTION_TURN", d=direction, p=pose, v=view, r=3)
                    pass
                pass
            pass

        #
        for pose in valid.pose:
            for view in valid.view:
                if view in ["UP"]: continue
                for direction in valid.direction:
                    index += 1
                    print(index)
                    valid.test(user_input="ACTION_MOVE", d=direction, p=pose, v=view, r=3)
                    pass
                pass
            pass
        pass
    except Exception as e:
        print("Error : ", e)
    pass
