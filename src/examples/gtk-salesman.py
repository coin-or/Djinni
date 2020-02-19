#!/usr/bin/env python

from djinni.tsp import TSP
from djinni.ca_tsp import Compressed, CA_TSP
import pygtk, re, sys, time
pygtk.require('2.0')
import gtk

class Point:
    def __init__(self, x, y, z):
        self.x = int(x)
        self.y = int(y)
        self.z = int(z)

class DumasReader:
    def __init__(self, name = "Dumas-1.set"):
        self.cities = []
        
        fh = open(name, "r")
        lines = fh.readlines()
        fh.close()
        lines = [x[:-1] for x in lines]
        regex = re.compile("^\s+\d+\s+(\d+)\.00\s+(\d+)\.00\s+\d+\.00\s+(\d+)\.00.*$")
        self.lowest_time = 99999
        self.highest_time = 0

        for x in lines:
            match = regex.match(x)
            if match and int(match.group(1)) != 999:

                point = Point(
                    match.group(1), 
                    match.group(2),
                    match.group(3))
                self.cities += [point]
                if point.z < self.lowest_time:
                    self.lowest_time = point.z
                if point.z > self.highest_time:
                    self.highest_time = point.z

        self.width = (self.highest_time - self.lowest_time) / 12.0


class MainWindow:
    def delete_event(self, widget, data = None):
        return False
        
    def destroy(self, widget, data = None):
        gtk.main_quit()
    
    def do_magic(self, widget, data = None):
        begintime = time.clock()
        self.ca_tsp.solve()
        endtime = time.clock()
                
        self.time.set_use_markup(True)
        self.time.set_markup("<span size='x-large'>" + str(endtime - begintime) + " sec</span>")
        
        answer = [int(x) for x in 
            self.ca_tsp.solution().lstrip().rstrip().split(" ")]
        cost = self.ca_tsp.cost()
        penalty = self.ca_tsp.penalty()
        
        self.cost.set_use_markup(True)
        self.cost.set_markup("<span size='x-large'>" + str(cost) + "</span>")
        
        span = "<span size='x-large'"
        if int(penalty) > 0:
            span += " foreground='red'"
        span += ">"
        
        self.penalty.set_use_markup(True)
        self.penalty.set_markup(span + str(penalty) + "</span>")
        self.solution = answer
        self.decorate_drawingarea(None, None)
        self.stepbutton.set_sensitive(True)
        self.searchbutton.set_sensitive(False)
        
    def decorate_drawingarea(self, widget, data = None):
        frac_x = (self.draw_x) / 50
        frac_y = (self.draw_y) / 50
        
        gdkwindow = self.drawingarea.window
        
        style = self.drawingarea.get_style()
        gc = gtk.gdk.GC(gdkwindow)
        gc = style.fg_gc[gtk.STATE_NORMAL]
        gc.line_width = 2
        colormap = gc.get_colormap()        
        black = colormap.alloc_color("black")

        for x in range(0, 51):
            x_coord = (x * frac_x)
            if x % 10 == 0:
                gdkwindow.draw_line(gc, x_coord, 0, x_coord, 6)
            else:
                gdkwindow.draw_line(gc, x_coord, 0, x_coord, 3)
        
        for y in range(0, 51):
            y_coord = (y * frac_y)
            if y % 10 == 0:
                gdkwindow.draw_line(gc, 0, y_coord, 6, y_coord)
            else:
                gdkwindow.draw_line(gc, 0, y_coord, 3, y_coord)

        colors = ["firebrick4", "firebrick3", "firebrick2", "firebrick1",
                  "green4", "green3", "green2", "green1",
                  "royalblue4", "royalblue3", "royalblue2", "royalblue1"]
        colors = [colormap.alloc_color(X) for X in colors]

        low = self.dumas.lowest_time
        width = self.dumas.width
        widths = []
        for index in range(1, 13):
            widths.append(low + (width * index))

        unified = [(widths[X], colors[X]) for X in range(12)]

        gc.set_foreground(black)
        for index in range(len(self.dumas.cities)):
            city = self.dumas.cities[index]
            x_coord = (city.x * frac_x)
            y_coord = (city.y * frac_y)
            gc.set_foreground(gtk.gdk.Color(gtk.gdk.color_parse("navajowhite").to_string()))
            for (width, color) in unified:
                if width > city.z:
                    gc.set_foreground(color)
                    break
            gdkwindow.draw_rectangle(gc, True,
                x_coord - 3, y_coord - 3, 6, 6)
        gc.set_foreground(black)
        if len(self.solution) > 0:
            last_point = self.dumas.cities[self.solution[0]]
            lesser = self.upto + 1
            if lesser > len(self.solution):
                lesser = len(self.solution)
            for x in range(1, lesser):
                next_point = self.dumas.cities[self.solution[x]]
                gdkwindow.draw_line(gc,
                    (last_point.x * frac_x),
                    (last_point.y * frac_y),
                    (next_point.x * frac_x),
                    (next_point.y * frac_y))
                last_point = next_point
        return True
    
    def step(self, widget, data = None):
        self.upto = self.upto + 7
        if self.upto > len(self.solution):
            self.stepbutton.set_sensitive(False)
            self.upto = len(self.solution)
        self.decorate_drawingarea(None, None)
        
    def __init__(self, filename = "Dumas-1.set"):
        self.upto = 0
        self.dumas = DumasReader(filename)
        
        self.tsp = TSP(filename)
        self.ca = Compressed(0.06, 0.0, 0.9999)
        self.ca_tsp = CA_TSP(self.ca, self.tsp, 
            0.95, 0.94, 75, 100, 30000)
        self.solution = []
        
        self.draw_x = 600
        self.draw_y = 600
        
        self.window = gtk.Window(gtk.WINDOW_TOPLEVEL)
        self.window.connect("delete_event", self.delete_event)
        self.window.connect("destroy", self.destroy)
        self.window.set_border_width(10)
        
        self.masterbox = gtk.VBox(False, 5)
        self.top_hbox = gtk.HBox(False, 5)
        self.top_left_table = gtk.Table(2, 3)
        
        self.cost_label = gtk.Label()
        self.cost_label.set_use_markup(True)
        self.cost_label.set_markup("<span size='x-large' weight='bold'>Cost:</span>")
        self.penalty_label = gtk.Label()
        self.penalty_label.set_use_markup(True)
        self.penalty_label.set_markup("<span size='x-large' weight='bold'>Penalty:</span>")
        self.cost = gtk.Label("")
        self.penalty = gtk.Label("")
        self.time_label = gtk.Label()
        self.time_label.set_use_markup(True)
        self.time_label.set_markup("<span size='x-large' weight='bold'>Elapsed Time:</span>")
        self.time = gtk.Label()
        self.time.set_text("")
        self.cost.set_alignment(0.5, 0.5)
        self.cost_label.set_alignment(1.0, 0.5)
        self.penalty.set_alignment(0.5, 0.5)
        self.penalty_label.set_alignment(1.0, 0.5)
        self.time_label.set_alignment(1.0, 0.5)
        self.time.set_alignment(0.5, 0.5)
        
        self.top_left_table.attach(self.cost_label, 0, 1, 0, 1)
        self.top_left_table.attach(self.cost, 1, 2, 0, 1)
        self.top_left_table.attach(self.penalty_label, 0, 1, 1, 2)
        self.top_left_table.attach(self.penalty, 1, 2, 1, 2)
        self.top_left_table.attach(self.time_label, 0, 1, 2, 3)
        self.top_left_table.attach(self.time, 1, 2, 2, 3)
        
        self.searchbutton = gtk.Button("Search")
        self.searchbutton.connect("clicked", self.do_magic, None)
        
        self.stepbutton = gtk.Button("Step")
        self.stepbutton.connect("clicked", self.step, None)
        self.stepbutton.set_sensitive(False)
        
        self.drawingarea = gtk.DrawingArea()
        self.drawingarea.set_size_request(self.draw_x, self.draw_y)
        self.drawingarea.connect("expose-event", self.decorate_drawingarea)
        
        self.mapframe = gtk.Frame("Roadmap")
        self.mapframe.add(self.drawingarea)
        
        self.top_hbox.pack_start(self.top_left_table)
        self.top_hbox.pack_start(self.searchbutton)
        self.top_hbox.pack_start(self.stepbutton)
        self.masterbox.pack_start(self.top_hbox)
        self.masterbox.pack_start(self.mapframe)
        self.window.add(self.masterbox)
        self.window.set_resizable(False)
        self.window.show_all()

    def main(self):
        gtk.main()

if __name__ == "__main__":
    foo = None
    if len(sys.argv) > 1:
        foo = MainWindow(sys.argv[1])
    else:
        foo = MainWindow()
    foo.main()
    
