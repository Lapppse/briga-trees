from os import getenv
from pathlib import Path

test_dir = Path.cwd() / "test"
result_dir = Path.cwd() / "result"
build_dir = Path.cwd() / "build"
input_file = Path.cwd() / "input.txt"
output_file = Path.cwd() / "output.txt"
measure_dir = Path.cwd() / "measure"
subdirs: list[str] = str(getenv("TEST_SUBDIR_NAMES")).split(";")
trees = str(getenv("TEST_TREE_NAMES")).split(";")
trees = ["set"]

test_priority = {"small": 0, "medium": 1, "large": 2}

for subdir_name in subdirs:
    print(subdir_name)
    for tree in trees:
        print(tree)
        for i in sorted((result_dir / subdir_name / tree).iterdir(), key=lambda x: test_priority[x.name]):
            mem_sum: float = 0
            time_sum: int = 0
            count = 0
            for j in i.iterdir():
                measure_file = measure_dir / subdir_name / tree / i.name / j.name
                mem_sum += float(measure_file.read_text().split(" ")[3])
                result_file = result_dir / subdir_name / tree / i.name / j.name
                time_sum += int(1_000_000_000 * float(result_file.read_text()))
                count += 1
            print(f"{i.name} time: {time_sum / count / 1_000_000_000} seconds mem: {mem_sum / count / 1024} MiB")
        print()
