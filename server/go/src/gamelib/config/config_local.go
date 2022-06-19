package config

import (
	"cmslib/utilc"
	"encoding/xml"
	"io/ioutil"
	"os"
	"path/filepath"
	"strings"
)

// svrno配置文件映射的
type SVRConfig struct {
	XMLName  xml.Name `xml:"app"` //tag
	Ip       string   `xml:"ip,attr"`
	Port     int      `xml:"port,attr"`
	ConfSrc  string   `xml:"confsrc,attr"`
	LogLevel string   `xml:"loglevel,attr"` //log等级
	LogDir   string   `xml:"logdir,attr"`   //log目录
	LogFile  string   `xml:"logfile,attr"`

	ExtParameters ExtParams `xml:"exts"`
	FileSrc       FileSrc   `xml:"filesrc"`
	HttpSrc       HttpSrc   `xml:"httpsrc"`
}

type ExtParamItem struct {
	XMLName xml.Name `xml:"ext"`

	K string `xml:"k,attr"`
	V string `xml:"v,attr"`
}
type ExtParams struct {
	XMLName xml.Name       `xml:"exts"`
	Items   []ExtParamItem `xml:"ext"`
}

type FileSrc struct {
	XMLName xml.Name `xml:"filesrc"`
	DataDir string   `xml:"datadir,attr"`
}

type HttpSrc struct {
	XMLName xml.Name `xml:"httpsrc"`
	Url     string   `xml:"url,attr"`
}

func loadSVRConfig(file string) (*SVRConfig, error) {
	dir, err := utilc.GetBinPath()
	if err != nil {
		return nil, err
	}

	fs := filepath.Join(dir, file)

	f, err := os.Open(fs)
	if err != nil {
		return nil, err
	}

	defer f.Close()

	data, err := ioutil.ReadAll(f)
	if err != nil {
		return nil, err
	}

	svr := SVRConfig{}
	err = xml.Unmarshal(data, &svr)
	if err != nil {
		return nil, err
	}

	return &svr, nil
}

func (s *SVRConfig) IsFileSrc() bool {
	return s.ConfSrc == "filesrc"
}

func (sc *SVRConfig) GetExtParams() (r map[string]string) {
	r = nil
	if len(sc.ExtParameters.Items) == 0 {
		return
	}

	r = make(map[string]string)
	for _, d := range sc.ExtParameters.Items {
		r[strings.ToLower(d.K)] = d.V
	}

	return
}
