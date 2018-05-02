var Smart = Smart || {};

Smart.MediaQuerie = {
    Orientations: {
        Landscape: 'landscape',
        Portrait: 'portrait'
    },
    Ratios: [
        {Name: 'square', Ratio: 1.25},
        {Name: 'laptop', Ratio: 1.6},
        {Name: 'laptop golden', Ratio: 1.618},
        {Name: 'widescreen', Ratio: 1.6667},
        {Name: 'widescreen hd-video', Ratio: 1.77},
        {Name: 'widescreen cinema', Ratio: 2.35},
        {Name: 'widescreen panorama', Ratio: 3}
    ],
    Breakpoints: [
        {Name: 'small', Width: 640},
        {Name: 'medium', Width: 1024},
        {Name: 'large', Width: 1200},
        {Name: 'large xlarge', Width: 1440},
        {Name: 'large xxlarge', Width: 1920}
    ],
    Current: {
        Size: 'medium',
        Orientation: 'landscape',
        Ratio: 'square',
        IsRetina: false
    },
    Resize: function (width, height, callback)
    {
        var prev = JSON.parse(JSON.stringify(this.Current));

        this.Current.Size = this.Breakpoints.last().Name;

        this.Breakpoints.forEach(function (i, item, _this) {
            if (item.Width >= width) {
                _this.Current.Size = item.Name;
                return false;
            }
            return true;
        }, this);

        var diff = width - height;
        this.Current.Orientation = (diff > 0)
                ? this.Orientations.Landscape
                : this.Orientations.Portrait;

        var ratio = Math.abs(diff > 0 ? width / height : height / width);

        this.Ratios.forEach(function (i, item, _this) {
            if (item.Ratio >= ratio) {
                _this.Current.Ratio = item.Name;
                return false;
            }
            return true;
        }, this);

        if (callback) {
            callback(prev, this.Current);
        }

        return this;
    }
};