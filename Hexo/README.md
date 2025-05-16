# Hexo 工具

### 编译使用

```bash
make all
```

### 配置文件

编译后可执行文件同目录下的 `config.yaml`

```yaml
similarityThreshold: 0.4
dependenciesSearchingFile: "package.json"
additionalTools:
  - ["swpp", "hexo swpp"]
  - ["gulp", "gulp zip"]
  - ["algolia", "hexo algolia"]

```

### 目前已有的功能：

- [x] 基本命令的自动化，让我们从繁琐的重复性工作中解放出来
- [x] 命令判断的高鲁棒性，妈妈再也不担心我的命令打错了
- [x] 高度的可调节性，可以根据自己的喜好调整工具行为
- [x] 命令输出的高价值密度，拒绝无用日志刷屏
- [x] 多彩的的日志输出，让我们能更直观地看到结果
- [x] 输出命令执行用时，更好的衡量性能

### 待实现的功能：

- [x] 进一步优化 `hexo server` 命令的异步执行
- [ ] 完善文档，拒绝让简单的事情变得复杂
- [ ] 输出命令执行结果并加以分析、建议
- [ ] 日志转储，更方便地追踪问题
- [x] 高兼容的依赖管理，`yarn`用户也不容忽视
- [ ] 适配 Windows 系统
- [ ] 日志级别

### 已知问题：

- [x] `hexo server` 命令在某些情况下会卡住，无法正常退出
- [ ] 本人缺少 `yarn` 的经验，请反馈问题
