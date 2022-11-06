# gosl

sl - but in Go, and in MULTIPLAYER

## Installation
```
go install github.com/Tondorf/gosl@v1.0.1
```

## Preparation of the server
```
gosl compile ~/go/pkg/mod/github.com/\!tondorf/gosl@v1.0.1/levels/locoworld
```

## Start of the server
```
gosl server -l locoworld.lvl
```

## Start of the client
```
gosl client -H <REMOTE_HOST<#ID>
```

