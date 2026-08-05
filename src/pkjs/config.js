module.exports = [
  {
    type: 'heading',
    defaultValue: 'Watchface Settings',
  },
  {
    type: 'section',
    items: [
      {
        type: 'radiogroup',
        messageKey: 'CATPPUCCIN_FLAVOR',
        label: 'Flavor',
        defaultValue: 'mocha',
        options: [
          { label: 'Latte', value: 'latte' },
          { label: 'Frappe', value: 'frappe' },
          { label: 'Macchiato', value: 'macchiato' },
          { label: 'Mocha', value: 'mocha' },
        ],
      },
      {
        type: 'toggle',
        messageKey: 'SHOW_HEARTRATE',
        label: 'Show Heart Rate',
        defaultValue: false,
        description: 'Show heart icon + BPM in the top-left corner'
      },
    ],
  },
  {
    type: 'submit',
    defaultValue: 'Save Settings',
  },
];
