# Shoestring

Shoestring is a complete framework for websites big and small. It includes:
- `softserve`: a handmade server
- `druid`: a front-end theme built on Bootstrap@5
- `module`: a simple and high-level build & CI system

Shoestring is modular. It integrates optionally, easily and out of the box with:
- GCP
- Docker
- Scss/Sass
- Bootstrap
- VCS (built-in)
- C++/PHP/Node.js/Go/etc.

It's the framework we built to manage [our website](https://edant.io). 

We hope you enjoy it.

## Licenses

The majority of this project are licensed permissively with the MIT License. There are things which this project uses (such as Bootstrap and Boost) which may have different, but always permissive, licenses.

## Getting started

*Optionally, for GCP deployment, create a cloudbuild.yaml by modifying cloudbuild.yaml.example with your preferences.*

0. clone this repository
1. run `./module help`
```
module
    --version [major.minor.patch]
    --source [build help --clean [hash cache out all]]
    browser|open
    attach|shell
    deploy|push
    quick|full
    build
    clean
    kill
    curl
    help
    run
```
2. run `./module -v 1.0.0 build run open`
3. edit for your needs and enjoy!

