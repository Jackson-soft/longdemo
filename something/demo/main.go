package main

import (
	"demo/reptile"
	"demo/web"
)

func main() {
	go reptile.Run()

	web.Run()
}
