#
# CMakeLists.txt for the whole OpenPT project
# File created on 2023/1/10
# Tianyu Huang <tianyu@illumiart.net>
#
#                    _ooOoo_
#                   o8888888o
#                   88" . "88
#                   (| -_- |)
#                   O\  =  /O
#                ____/`---'\____
#              .'  \\|     |//  `.
#             /  \\|||  :  |||//  \
#            /  _||||| -:- |||||-  \
#            |   | \\\  -  /// |   |
#            | \_|  ''\---/''  |   |
#            \  .-\__  `-`  ___/-. /
#          ___`. .'  /--.--\  `. . __
#       ."" '<  `.___\_<|>_/___.'  >'"".
#      | | :  `- \`.;`\ _ /`;.`/ - ` : | |
#      \  \ `-.   \_ __\ /__ _/   .-` /  /
# ======`-.____`-.___\_____/___.-`____.-'======
#                    `=---='
# ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
#             Buddha bless, never bug.

cmake_minimum_required(VERSION 3.10)
project(OPENPT LANGUAGES CXX VERSION 0.1)

set(cmake_cxx_standard 17)
set(cmake_cxx_standard_required True)

add_subdirectory(src)
add_subdirectory(tests)
