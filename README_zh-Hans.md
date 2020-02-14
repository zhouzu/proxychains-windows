# Proxychains.exe 自述文件

[README](README.md) | [简体中文文档](README_zh-Hans.md)

Proxychains.exe 是一个适用于 Win32(Windows) 和 Cygwin 平台的命令行强制代理工具
（Proxifier）。它能够截获大多数 Win32 或 Cygwin 程序的 TCP 连接，强制它们通过
一个或多个 SOCKS5 代理隧道。

Proxychains.exe 通过给动态链接的程序注入一个 DLL，对 Ws2_32.dll 的 Winsock 函数
挂钩子的方式来将应用程序的连接重定向到 SOCKS5 代理。

Proxychains.exe 是 [proxychains4](https://github.com/haad/proxychains) 或者
[proxychains-ng](https://github.com/rofl0r/proxychains-ng) 到 Win32 和 Cygwin 的
移植产物。它也使用了 [uthash](https://github.com/troydhanson/uthash) 构建一些
数据结构，以及使用了 [minhook](https://github.com/TsudaKageyu/minhook) 进行 API
的挂钩。

警告：此程序只对动态链接的程序有用。同时，Proxychains.exe 和需要运行的目标程序
必须是同一架构和平台（用 proxychains_x86.exe 运行 x86 程序，用
proxychains_x64.exe 运行 x64 程序；用 Cygwin 下构建的版本来运行 Cygwin 程序）。

警告：此程序是基于 Hack 的，并且处于开发早期阶段。使用过程中可能会发生任何意外
状况。被运行的程序可能会崩溃、无法工作、产生意想不到的运行结果等等。谨慎使用。

警告：此程序可能用于绕过审查。此举在某些国家或地区可能是危险、不符合法律的。

**请在用于正式用途前，确保本程序和代理正常工作。**

比如，你可以通过连接到一些查询本机 IP 的服务如 ifconfig.me ，确保你未暴露你的
真实 IP 地址。

**请在确保清楚你要执行的操作及其后果后使用本程序。**

**免责声明：本程序的作者不对任何滥用、误用此软件的行为以及其可能导致的后果
负责。**

# 构建

## 构建 Win32 版本

使用较新版本的 Visual Studio 打开 proxychains.exe.sln （Visual Studio 2019 测试
有效）。Visual Studio 应该安装 v141_xp 平台工具集。构建整个解决方案，在默认位置
（如 x64\Debug）找到输出的 EXE 和 DLL 文件。

## 构建 Cygwin 版本

安装 Cygwin 和各种构建工具程序包（gcc、w32api-headers、w32api-runtime 等）。
运行 Cygwin bash，切换到 `cygwin-build` 目录下，执行 `make`。

# 安装

把生成的 `proxychains*.exe` 和 `[cyg]proxychains_hook*.dll` 复制到 `PATH` 环境
变量包含的某个目录下。另外你还需要在正确的位置创建配置文件。参见“配置”。

# 配置

Proxychains.exe 按照以下顺序寻找配置：

- 环境变量 `%PROXYCHAINS_CONF_FILE%` 或 `$PROXYCHAINS_CONF_FILE` 或通过 -f 命令
  行参数指定的文件
- $HOME/.proxychains/proxychains.conf （Cygwin 用户主目录） 或
  %USERPROFILE%\.proxychains\proxychains.conf （Win32 用户主目录）
- (SYSCONFDIR)/proxychains.conf （Cygwin） or
  (用户的 Roaming 目录)\Proxychains\proxychains.conf （Win32）
- /etc/proxychains.conf （Cygwin） or
  (全局的 ProgramData 目录)\Proxychains\proxychains.conf （Win32）
  
关于配置选项，参见 `proxychains.conf`

# 用例

`proxychains ssh some-server`

`proxychains SomePath\firefox.exe`

运行 `proxychains -h` 查看更多命令行参数选项。

# 工作原理

- 主程序 Hook `CreateProcessW` Win32 API 函数调用。
- 主程序创建按照用户给定的命令行启动子进程。
- 创建进程后，挂钩后的 `CreateProcessW` 函数将 Hook DLL 注入到子进程。当子进程
  被注入后，它也会 Hook 如下的 Win32 API 函数调用：
  - `CreateProcessW`，这样每一个后代进程都会被注入；
  - `connect` 和 `ConnectEx`，这样就劫持了 TCP 连接；
  - `GetAddrInfoW` 系列函数，这样可以使用 Fake IP 来追踪访问的域名，用于远程
    DNS 解析；
  - 等等。
- 主程序并不退出，而是作为一个命名管道服务端存在。子进程与主程序通过命名管道
  交换包括日志、域名等内容在内的数据。主程序实施大多数关于 Fake IP 和子进程是
  否还存在的簿记工作。
- 当所有后代进程退出后，主程序退出。
- 主程序收到一个 SIGINT（Ctrl-C）后，终止所有后代进程。

# To Do

详见英文文档。

# 授权协议

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License version 2 as 
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.