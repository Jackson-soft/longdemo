package web

import "github.com/gin-gonic/gin"

func Run() {
	r := gin.Default()

	r.Run("9088")
}