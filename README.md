# cwlpro

## 项目介绍

这是一个用于测试 CI/CD 流程的示例项目，主要功能是通过 GitHub Actions 实现代码自动编译、测试和部署到 CentOS 服务器。

## 环境要求

### 开发环境
- 操作系统：Windows/macOS/Linux
- 开发工具：Visual Studio Code 或其他 C++ 开发环境
- Git：用于版本控制

### CI/CD 环境
- GitHub Actions：用于自动构建和部署
- Ubuntu 22.04 (GitHub Actions 运行环境)
- CentOS 7/8 (目标部署服务器)

### 依赖
- C++ 编译器 (g++)
- Make 工具
- SSH (用于服务器连接)
- sshpass (用于自动化密码认证)

## 项目结构

```
cwlpro/
├── .github/
│   └── workflows/
│       ├── build_deploy.yml   # 构建并部署到 CentOS 服务器的工作流
│       └── test.yml           # 构建和测试工作流
├── code/
│   └── c_demo.cpp            # 示例 C++ 代码
└── README.md                 # 项目说明文档
```

## CI/CD 工作流说明

### 1. test.yml (构建和测试工作流)

**触发条件**：
- 向 `stable_001` 分支推送代码

**工作流程**：
1. 检出代码
2. 安装依赖
3. 使用静态链接编译代码生成可执行文件
4. 运行编译后的程序进行测试
5. 上传编译产物

### 2. build_deploy.yml (构建并部署工作流)

**触发条件**：
- 推送以 `stable001` 开头的标签

**工作流程**：
1. 检出代码
2. 安装依赖
3. 使用静态链接编译代码生成可执行文件
4. 在构建环境中运行测试程序
5. 安装 sshpass 用于自动化部署
6. 连接到 CentOS 服务器
7. 创建部署目录（如果不存在）
8. 在 CentOS 服务器上安装必要的依赖包
9. 上传编译好的可执行文件
10. 在服务器上运行程序

## 使用流程

### 1. 准备工作

#### GitHub 仓库配置
- 确保仓库已启用 GitHub Actions
- 在仓库的 "Settings" > "Secrets and variables" > "Actions" 中配置以下 Secrets：
  - `CSEVNAME`：CentOS 服务器用户名
  - `HOST2_IP`：CentOS 服务器 IP 或域名
  - `HOST2_PW`：CentOS 服务器密码

#### 服务器配置
- 确保 CentOS 服务器已开启 SSH 服务
- 确保服务器有足够的权限创建和执行文件

### 2. 开发和提交代码

1. 克隆仓库：
   ```bash
   git clone https://github.com/wanglaichen/cwlpro.git
   cd cwlpro
   ```

2. 切换到开发分支：
   ```bash
   git checkout stable_001
   ```

3. 修改 `code/c_demo.cpp` 文件

4. 提交并推送代码：
   ```bash
   git add code/c_demo.cpp
   git commit -m "描述你的修改"
   git push origin stable_001
   ```

### 3. 触发自动构建和部署

#### 方法一：推送代码到 stable_001 分支
推送代码到 `stable_001` 分支会触发 `test.yml` 工作流：
- `test.yml`：构建并测试代码，上传编译产物

#### 方法二：创建并推送标签

1. 创建标签：
   ```bash
   git tag stable001_v1.0.0
   ```

2. 推送标签：
   ```bash
   git push origin stable001_v1.0.0
   ```

这将触发 `build_deploy.yml` 工作流进行构建和部署。

### 4. 查看 CI/CD 运行状态

1. 登录 GitHub
2. 进入仓库页面
3. 点击顶部的 "Actions" 标签页
4. 查看当前运行的工作流及其状态

## 静态链接说明

项目使用静态链接编译代码，以解决 GLIBC 版本不兼容问题：

```bash
g++ -static -o release_stable001 c_demo.cpp
```

这种方式会将所有依赖库打包到可执行文件中，确保程序可以在不同 GLIBC 版本的 CentOS 服务器上运行。

## 注意事项

1. **安全问题**：项目使用 sshpass 进行密码认证，建议在生产环境中使用 SSH 密钥认证
2. **超时设置**：工作流设置了超时时间，长时间运行的任务可能会被中断
3. **资源限制**：GitHub Actions 有资源使用限制，大量并行构建可能会受到限制
4. **环境差异**：注意开发环境和部署环境的差异，确保静态链接能解决所有依赖问题

## 故障排除

### 常见问题

1. **工作流未触发**
   - 检查分支名称是否正确
   - 检查标签格式是否符合要求
   - 检查 GitHub Actions 是否在仓库中启用

2. **构建失败**
   - 检查代码语法错误
   - 检查依赖是否正确安装
   - 检查编译命令是否正确

3. **部署失败**
   - 检查服务器连接信息是否正确
   - 检查服务器 SSH 服务是否正常运行
   - 检查服务器权限是否足够
   - 检查 Secrets 配置是否正确

### 日志查看

工作流运行失败时，可以查看详细日志：
1. 在 "Actions" 页面找到失败的工作流
2. 点击进入查看详情
3. 展开失败的步骤查看详细日志

## 更新日志

- v1.0.0：初始版本，实现基本的 CI/CD 功能
- 增加静态链接编译，解决 GLIBC 版本不兼容问题
- 优化工作流触发条件