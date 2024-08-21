#!/usr/bin/env python3

modes = {'encrypt': 0, 'decrypt': 1}

limited = False


def to_compound_str(val):
    assert len(val) % 2 == 0, "Only even sized values supported"
    if len(val) > 0:
        import re
        a = re.findall('..', val)
        b = "(const uint8_t []){"
        for s in a:
            b += "0x" + s + ", "
        b += "}, " + repr(len(val) / 2) + ","
    else:
        b = "NULL, 0,"
    return b


def generate_case(outf, myvars, mode):
    if 'PT' not in myvars:
        myvars['PT'] = ''
    if 'FAIL' in myvars:
        return
    if limited and myvars['Count'] != '0':
        return
    # Skip cases not supported by GP
    if len(myvars['Tag']) / 2 < 96 / 8:
        return

    outf.write('{ TEE_ALG_AES_GCM, ' + mode + ', TEE_TYPE_AES,\n')
    outf.write('/* Key */ ' + to_compound_str(myvars['Key']) + '\n')
    outf.write('/* IV  */ ' + to_compound_str(myvars['IV']) + '\n')
    outf.write('0,\n')
    outf.write('/* AAD */ ' + to_compound_str(myvars['AAD']) + '\n')
    outf.write('0,\n')
    outf.write('/* PT  */ ' + to_compound_str(myvars['PT']) + '\n')
    outf.write('/* CT  */ ' + to_compound_str(myvars['CT']) + '\n')
    outf.write('/* Tag */ ' + to_compound_str(myvars['Tag']) + '\n')
    outf.write(repr(myvars['Line']) + '},\n')


def get_args():
    import argparse

    parser = argparse.ArgumentParser()

    parser.add_argument('--inf', required=True,
                        type=argparse.FileType('r'),
                        help='Name of input RSP file')

    parser.add_argument('--outf', required=True,
                        type=argparse.FileType('w'),
                        help='Name of output C file')

    parser.add_argument('--mode', required=True, choices=modes.keys(),
                        help='mode: encrypt or decrypt')

    parser.add_argument('--limited', action="store_true",
                        help='Only run one test case from each group')

    return parser.parse_args()


def main():
    import re
    global limited
    args = get_args()
    inf = args.inf
    outf = args.outf
    myvars = {}
    line_num = 0

    if args.mode == "encrypt":
        mode = "TEE_MODE_ENCRYPT"
    else:
        mode = "TEE_MODE_DECRYPT"

    limited = args.limited

    for line in inf:
        line_num += 1
        myl = line.strip()
        if len(myl) == 0:
            continue
        if re.match('^#', myl):
            continue
        if re.match('^\[', myl):
            continue
        s = re.split('\W+', myl)
        if len(s) == 0:
            continue
        name = s[0]
        if name == 'Count':
            if len(myvars) > 1:
                generate_case(outf, myvars, mode)
                myvars = {}
            myvars['Line'] = line_num

        if len(s) < 2:
            myvars[s[0]] = ''
        else:
            myvars[s[0]] = s[1]

        if len(s) < 2:
            continue
        val = s[1]

    if len(myvars) > 1:
        generate_case(outf, myvars, mode)


if __name__ == "__main__":
    main()
