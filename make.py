#!/bin/python3
# Make.py
import yaml
import argparse
import os
import subprocess


parser: argparse.ArgumentParser = argparse.ArgumentParser()
parser.add_argument("--init", action="store_true", help="Initialize in wd")
parser.add_argument("--build", type=str, help="Build a specific target")


class ConstantProvider:
    constants: dict = {
        "config_file": "makecfg.yml"
    }

    def __init__(self):
        pass

    @staticmethod
    def get(key) -> str:
        return ConstantProvider.constants[key]


class Config:
    data: dict = {}

    def __init__(self):
        pass


class ConfigVariableManager:

    def __init__(self):
        self.variables: dict[str, str] = {}

    def add(self, key: str, val: str) -> None:
        self.variables[key] = val

    def get(self, key: str) -> str:
        return self.variables.get(key, "")

    def replace_str_with_variables(self, string: str, max_iters: int = 10) -> str:
        """
        Replace placeholders ${VAR} recursively until they are fully resolved
        or until max_iters is reached to prevent infinite loops.
        """
        result = string
        for _ in range(max_iters):
            changed = False
            for k, v in self.variables.items():
                placeholder = f"${{{k}}}"
                if v.startswith("${mpy_exec ") and v.endswith("}"):
                    command = v[11:-1]
                    command = command
                    proc = subprocess.run(
                        command.split(),
                        capture_output=True,
                        text=True
                    )
                    result = result.replace(v, proc.stdout)
                elif placeholder in result:
                    result = result.replace(placeholder, str(v))
                    changed = True
            if not changed:
                break
        return result

    def load_dict(self, dict_: dict[str, str]) -> None:
        """
        Load variables and resolve placeholders recursively.
        """
        self.variables = dict(dict_)  # copy
        for k, v in self.variables.items():
            self.variables[k] = self.replace_str_with_variables(v)


def parse_build_str(build_str: str) -> list[str]:
    data: list[str] = []
    for part in build_str.split(","):
        data.append(part.strip())

    return data


version: float = 10.2


def main():
    args = parser.parse_args()
    if args.init:
        if not os.path.exists(ConstantProvider.get("config_file")):
            os.system("cp /home/noerlol/.scripts/resources/makecfg.yml .")
            print("Generated a config file")
    if not os.path.exists(ConstantProvider.get("config_file")):
        print(f"Config file not found: {ConstantProvider.get('config_file')}")
        return
    if not args.build:
        return
    config: Config = Config()
    cfg_variables: ConfigVariableManager = ConfigVariableManager()
    with open(ConstantProvider.get("config_file"), "r") as f:
        config.data = yaml.load(f, Loader=yaml.FullLoader)

    cfg_variables.load_dict(config.data["variables"])
    # Add Pre-Loaded variables
    cfg_variables.add("PROJECT_DIRECTORY", os.getcwd())
    cfg_variables.add("MPY_VERSION", str(version))
    cfg_variables.add("MPY_EXEC_SUPPORTED", "true")

    try:
        if args.build is not None:
            tasks = parse_build_str(args.build)
            if "all" in tasks:
                tasks = config.data["tasks"].keys()
                print("--> Running all tasks")
            if not all(task in config.data["tasks"] for task in tasks):
                tasks_not_found = []
                for task in tasks:
                    if task not in config.data["tasks"]:
                        tasks_not_found.append(task)
                print(f"Tasks not found: {tasks_not_found}")
            else:
                for task in tasks:
                    task_data = config.data["tasks"][task]
                    name = task_data.get("metadata", {}).get("name", task)
                    print(f"--> Running Task: {name}")
                    for command in config.data["tasks"][task]["commands"]:
                        os.system(cfg_variables.replace_str_with_variables(command))
    except KeyboardInterrupt:
        print("Interrupted by user")
        exit(67)


if __name__ == "__main__":
    init_str: str = f"make.py v{version}"
    print(init_str)
    main()
