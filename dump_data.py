"""Dumps data from the entity database, used by hooking framework and importing IDA status."""


import csv
import argparse
import logging
from reccmp.isledecomp.types import EntityType
from reccmp.isledecomp.compare import Compare
from reccmp.project.detect import (
    RecCmpProjectException,
    argparse_add_project_target_args,
    argparse_parse_project_target,
)
from reccmp.project.detect import __file__ as detect_py_file_name 


# Ignore all compare-db messages.
logging.getLogger("reccmp.isledecomp.compare").addHandler(logging.NullHandler())

# FIXME: The logger in this file should use __name__ instead of __file__.
logging.getLogger(detect_py_file_name).addHandler(logging.NullHandler())


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        allow_abbrev=False,
        description="Custom data export.",
    )

    argparse_add_project_target_args(parser)
    args = parser.parse_args()

    return args

def entity_handler(ent, csv_writer):
    # Assumes ent has orig_addr and entity_type
    type_enum = EntityType(ent.entity_type)

    row = [
        f"{ent.orig_addr:x}",           # Hex address
        ent.matched,                    # Boolean
        bool(ent.get("stub")),          # Boolean
        type_enum.name,                 # Enum name
        ent.name,                       # Entity name
        ent.get("symbol")               # Mangled symbol
    ]

    csv_writer.writerow(row)


def main():
    logger = logging.getLogger()

    args = parse_args()
    try:
        target = argparse_parse_project_target(args)
    except RecCmpProjectException as e:
        logger.error("%s", e.args[0])
        return 1

    csv_file = open("symbols.csv", "w", newline="")
    csv_writer = csv.writer(csv_file)

    # Write header row (optional)
    #csv_writer.writerow(["orig_addr", "matched", "stub", "entity_type", "name", "symbol"])

    compare = Compare.from_target(target)

    for ent in compare.get_all():
        if ent.entity_type is None or ent.entity_type not in (EntityType.FUNCTION, EntityType.DATA):
            continue

        if ent.orig_addr is None:
            continue

        entity_handler(ent, csv_writer)

if __name__ == "__main__":
    main()