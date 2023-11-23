# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

# def options(opt):
#     pass

# def configure(conf):
#     conf.check_nonfatal(header_name='stdint.h', define_name='HAVE_STDINT_H')

def build(bld):
    module = bld.create_ns3_module('leo', ['core','internet', 'propagation', 'stats', 'traffic', 'flow-monitor', 'applications'])
    module.source = [
        'model/leo.cc',
        'model/isl-net-device.cc',
        'model/isl-channel.cc',
        'model/isl-propagation-loss-model.cc',
        'model/leo-mobility-model.cc',
        'helper/leo-helper.cc',
        'helper/isl-helper.cc',
        ]

    module_test = bld.create_ns3_module_test_library('leo')
    module_test.source = [
        'test/leo-test-suite.cc',
        ]

    headers = bld(features='ns3header')
    headers.module = 'leo'
    headers.source = [
        'model/leo.h',
        'model/isl-net-device.h',
        'model/isl-channel.h',
        'model/isl-propagation-loss-model.h',
        'model/leo-mobility-model.h',
        'helper/leo-helper.h',
        'helper/isl-helper.h',
        ]

    if bld.env.ENABLE_EXAMPLES:
        bld.recurse('examples')

    # bld.ns3_python_bindings()

