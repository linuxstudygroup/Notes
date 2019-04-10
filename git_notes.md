# Git
- 工作区 -> add -> 暂存区 -> commit -> 版本库
   ![git command](git_command.jpg)
- git workflow
   ![git workflow](git_flow.png)
- git merge 与 git rebase
   ![git merge and git rebase](https://img-my.csdn.net/uploads/201206/14/1339683149_4793.jpg)

- ***Git跟踪并管理的是修改，而非文件***
- `git checkout` 
   - `git checkout -- file_name` 可以丢弃 ***工作区*** 的修改, 让这个文件回到最近一次 `git commit` 或 `git add` 时的状态
   - `git checkout -b branch_name` 创建并切换到该分支
- `git merge`
   - fast-forward 删除分支后，会丢掉分支信息
   - --no-ff Git就会在merge时生成一个新的commit，这样，从分支历史上就可以看出分支信息
- `git reset`
   - `git reset HEAD file` 可以把 ***暂存区*** 的修改撤销掉(unstage)，重新放回工作区
   - `git reset --hard commit_id` 版本回退
- `git stash`
   - `git stash`
   - `git stash list`
   - `git stash apply`
   - `git stash drop`
   - `git stash pop` = `git stash apply` + `git stash drop`
- `git rebase`
   - 不要通过rebase对任何已经提交到公共仓库中的commit进行修改（你自己一个人玩的分支除外）
   [git 重写历史1](https://git-scm.com/book/zh/v1/Git-工具-重写历史),
   [git 重写历史2](https://thoughtbot.com/blog/git-interactive-rebase-squash-amend-rewriting-history)
   - `git rebase -i  [startpoint]  [endpoint]` 合并多个commit为一个完整commit.其中-i的意思是--interactive，即弹出交互式的界面让用户编辑完成合并操作，[startpoint]  [endpoint]则指定了一个编辑区间，如果不指定[endpoint]，则该区间的终点默认是当前分支HEAD所指向的commit
   - `git rebase   [startpoint]   [endpoint]  --onto  [branchName]` 将某一段commit粘贴到另一个分支上
- `git tag`
   - tag是和commit绑定的
   - `git tag -a tag_name -m "tag description" [commit_id]`
   - `git tag` 查看标签
   - `git show tag_name`
   - `git tag -d tag_name`
   - `git push origin --tags`
   - `git push origin tag_name`
   - `git push origin `
- `git reflog`
   - 查看历史命令
- `git log`
   - `git log --graph --pretty=oneline --abbrev-commit` 可以看到分支合并图
- `git rm`
   - 从版本库中删除某个文件
- .gitignore
   - `git add -f`
   - `git check-ignore`
- `git config --global alias.xxx 'xxx'`