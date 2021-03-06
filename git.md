# Git Command on Terminal

## Initial Setup

* Install git

```bash
sudo apt install git
```

* Configuration

```bash
git config --global user.name user_name_here
git config --global user.password user_password_here
```

---

## Add Files

* Create folder

```bash 
# at your local directory. eg: /home/USERNAME
mkdir test
cd test
# initialization
git init
```

* Add file

```bash
touch test.c

# add "test.c" into buffer
git add test.c

vim test.c
hello world!
:wq
```

* Commit

```bash
# push to repo
# simply state what you have done
git commit -m "add new file \"test.c\""

# rewrite commit
git commit --amend
```

---

## Create new branch 

Some examples: 

* `mem_overflow`
* `idea`
* `new_feature`

```bash
# Create branch
git branch dev

# Move to branch
git checkout dev

# Or just one command line
git checkout -b dev

# list branch
git branch
```

---

## Useful `git ` command

### Log

```bash
git log

# display in short sentence
git log --pretty=oneline
# or
git log --oneline
```

### Rollback

```bash
# 3 options
# --soft	rollback but not include something added before
# --mixed	rollback something added before
# --hard	rollback all
git reset [option]

# Or
git reset --hard HEAD^
# ^ : latest version of master

git reset --hard HEAD~3
# ~3 : third version of master
```

### Status

Check change made by another person

```bash
git status
```

### Remove file

```bash
rm test.c
# or 
git
```

### Review Last Commit

```bash
# last commit
git show 

# last 2 commit
git show HEAD~2

# === git diff ===
git diff COMMIT_ID-1 COMMIT_ID-2
```

### Merge vs Rebase

```bash
# at main branch
git merge feature_branch
# main branch is updated

# at feature branch
git rebase main
# feature branch is updated
```



---

# GitHub - Remote Repository

## Initial Setup on Terminal

* Generate ssh key for your device

```bash
# set username & passphrase
git config --global user.name user_name_here
git config --global user.password user_password_here

# Generate secret key
ssh-keygen -t rsa -C "seventan1234@gmail.com"

# -t	secret key. Default=rsa
# -C	description. eg: email
# -f 	particular file. default: /home/USERNAME/.ssh/id_rsa

# Then select the file to be share
# Enter passphrase
```

* Key: id_rsa

```bash
ls ~/.ssh
# Output: $ id_rsa	 	id_rsa.pub
# Whatis: 	private		public key

# copy public key to github(settings > SSH & GPG keys)
```

* Test ssh connection

```bash 
ssh -T git@github.com
```

Then shell would prompt out:

`Enter passphrase for key '/home/seventan/.ssh/id_rsa':  
Hi seventan1234! You've successfully authenticated, but GitHub does not provide shell access.
 `

## `git remote`

```bash
git init
git remote add origin git@github.com:seventan1234/new.git

# Check remote origin
git remote -v

# If want to remove
git remote remove orgin

# If want to view
git remote show origin
```

---

## Basic Git Operation - `pull` & `push`

* To copy repo from GitHub

```bash
git clone git@github.com:seventan1234/new.git
```

* Track **local** repo **from remote** repo

```bash
git pull git@github.com:seventan1234/new.git
```

* Push your file to repo

```bash
git push -u origin master
```

---

