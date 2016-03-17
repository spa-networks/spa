#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import argparse
import math


def stddev(seq, xbar):
    return math.sqrt(sum([(x - xbar) ** 2 for x in seq]) / (len(seq) - 1))


def mean(seq):
    return sum(seq) / len(seq)


def main():
    prs = argparse.ArgumentParser(description='Take SPA* complete internal\
                                               degree output and output the\
                                               community size vs average\
                                               internal degree relation.')
    prs.add_argument('files', type=str, nargs='+',
                     help='SPA* complete internal degree output.')
    args = prs.parse_args()

    for file_path in args.files:
        with open(file_path, 'r') as f:
            # Classify degree sequences in proper compartments (by size)
            k_sequences = dict()
            for line in f:
                degrees = [int(x) for x in line.strip().split(" ")]
                try:
                    k_sequences[len(degrees)] += degrees
                except KeyError:
                    k_sequences[len(degrees)] = degrees
            # Compute averages and deviations
            for size in sorted(k_sequences):
                avg = mean(k_sequences[size])
                lower_list = [x for x in k_sequences[size] if x <= avg]
                upper_list = [x for x in k_sequences[size] if x > avg]
                if len(lower_list) > 1:
                    lower_stdev = stddev(lower_list, xbar=avg)
                else:
                    lower_stdev = 0
                if len(upper_list) > 1:
                    upper_stdev = stddev(upper_list, xbar=avg)
                else:
                    upper_stdev = 0
                print(size, avg, lower_stdev, upper_stdev)

if __name__ == '__main__':
    main()
